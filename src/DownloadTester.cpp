#include "DownloadTester.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

size_t DownloadTester::download_write_callback(void* contents, size_t size, size_t nmemb,
                                               void* userp) {
    auto* tester = static_cast<DownloadTester*>(userp);
    long long current = tester->total_downloaded_bytes.load();
    long long limit = tester->max_download_bytes.load();
    size_t bytes = size * nmemb;

    // stop BEFORE exceeding limit
    if (limit > 0 && current >= limit) {
        tester->stop_requested = true;
        return 0;
    }

    // clamp last chunk
    if (limit > 0 && current + bytes > limit) {
        bytes = static_cast<size_t>(limit - current);
    }

    if (tester->stop_requested.load()) {
        return 0;
    }

    tester->total_downloaded_bytes += bytes;
    return bytes;
}

int DownloadTester::download_progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                               curl_off_t ultotal, curl_off_t ulnow) {
    auto* tester = static_cast<DownloadTester*>(clientp);

    if (tester->stop_requested) {
        return 1;
    }

    return 0;
}

void DownloadTester::worker() {
    CURL* curl;
    const char* url = config.download_url.c_str();
    curl = curl_easy_init();

    if (curl) {
        // tells libcurl WHAT to download
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // tells libcurl HOW to handle incoming data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_write_callback);

        // passes custom user data into write callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        // install progress/control callback
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, download_progress_callback);

        // pass object pointer into progress callback
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);

        // enable progress callback system
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        // Enables automatic HTTP redirect following
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Pretending to be a browser often fixes: blocking, throttling, denied requests
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

        CURLcode res = curl_easy_perform(curl);

        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (res == CURLE_OK) {
            lock_guard<mutex> lock(console_mutex);
            cout << "HTTP Response: " << response_code << "\n";
        } else if (res == CURLE_ABORTED_BY_CALLBACK || res == CURLE_WRITE_ERROR) {
            /*
                Print for debugging
                lock_guard<mutex> lock(console_mutex);
                cout << "Transfer stopped intentionally\n";
            */
        } else {
            lock_guard<mutex> lock(console_mutex);
            cout << "Curl Error: " << curl_easy_strerror(res) << "\n";
            cout << "HTTP Response: " << response_code << "\n";
        }

        curl_easy_cleanup(curl);
    }
}

DownloadResult DownloadTester::run(const SpeedTestConfig& cfg) {
    DownloadResult result{};
    vector<thread> threads;

    config = cfg;
    total_downloaded_bytes = 0;
    max_download_bytes = config.download_payload_mb * 1024LL * 1024LL;
    stop_requested = false;

    cout << "Starting timed download test...\n";

    auto total_start = high_resolution_clock::now();
    monitor.start(&total_downloaded_bytes, &stop_requested, &sampler);
    for (int i = 0; i < config.thread_count; i++) {
        threads.emplace_back([this]() { worker(); });
    }
    this_thread::sleep_for(seconds(config.test_duration_seconds));
    stop_requested = true;

    cout << "Stopping downloads...\n";

    for (auto& t : threads) {
        t.join();
    }
    monitor.join();

    auto total_end = high_resolution_clock::now();
    double total_seconds = duration_cast<duration<double>>(total_end - total_start).count();
    long long bytes = total_downloaded_bytes.load();
    double total_megabits = (bytes * 8.0) / 1e6;
    double aggregate_speed = total_megabits / total_seconds;

    result.downloaded_mb = bytes / (1024.0 * 1024.0);
    result.wall_time_seconds = total_seconds;
    result.aggregate_speed_mbps = aggregate_speed;
    result.smoothed_speed = sampler.smoothed();
    result.running_average_speed = sampler.running_average();
    result.min_speed = sampler.minimum();
    result.max_speed = sampler.maximum();
    result.standard_deviation = sampler.stddev();
    result.p5 = sampler.percentile(0.05);
    result.p50 = sampler.percentile(0.50);
    result.p95 = sampler.percentile(0.95);
    result.p99 = sampler.percentile(0.99);

    return result;
}
