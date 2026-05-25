#include "UploadTester.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <thread>

using namespace std;
using namespace std::chrono;

size_t UploadTester::read_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* context = static_cast<UploadContext*>(userdata);
    if (!context) return CURL_READFUNC_ABORT;
    if (context->offset >= context->total_size) return 0;

    size_t max_bytes = size * nmemb;
    size_t remaining = context->total_size - context->offset;
    size_t bytes_to_copy = min(max_bytes, remaining);
    if (bytes_to_copy == 0) return 0;

    memcpy(ptr, context->buffer.data() + context->offset, bytes_to_copy);
    context->offset += bytes_to_copy;
    context->tester->total_uploaded_bytes += bytes_to_copy;

    return bytes_to_copy;
}

int UploadTester::progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                    curl_off_t ultotal, curl_off_t ulnow) {
    auto* tester = static_cast<UploadTester*>(clientp);

    if (tester->stop_requested) {
        return 1;
    }

    return 0;
}

void UploadTester::worker() {
    CURL* curl = curl_easy_init();

    if (!curl) {
        lock_guard<mutex> lock(console_mutex);

        cout << "Failed to initialize curl\n";

        return;
    }

    // Create synthetic upload payload

    UploadContext context;

    context.total_size = config.upload_payload_mb * 1024 * 1024;

    context.buffer.resize(context.total_size, 'A');

    // Connect callback context back to UploadTester
    // so callback can update sampler counters
    context.tester = this;

    // Configure upload request

    curl_easy_setopt(curl, CURLOPT_URL, config.upload_url.c_str());

    // Enable HTTP POST upload
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Upload callback
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    // Pass upload context
    curl_easy_setopt(curl, CURLOPT_READDATA, &context);

    // Total upload size
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)context.total_size);

    // Follow redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Browser-like user agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    // Optional:
    // discard server response body
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        +[](void* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
            return size * nmemb;
        });

    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);

    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    while (!stop_requested) {
        // Reset upload stream state
        // Previous upload exhausted the stream,
        // so offset must return to beginning
        context.offset = 0;

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_ABORTED_BY_CALLBACK) {
            // Expected cooperative shutdown
        } else if (res != CURLE_OK) {
            lock_guard<mutex> lock(console_mutex);

            cout << "Curl Error: " << curl_easy_strerror(res) << "\n";
        }
    }

    curl_easy_cleanup(curl);
}

UploadResult UploadTester::run(const SpeedTestConfig& cfg) {
    UploadResult result{};

    config = cfg;
    total_uploaded_bytes.store(0);
    stop_requested = false;

    cout << "Starting timed upload test...\n";

    auto total_start = high_resolution_clock::now();
    monitor.start(&total_uploaded_bytes, &stop_requested, &sampler);

    // Start upload workers
    vector<thread> workers;
    for (int i = 0; i < config.thread_count; i++) {
        workers.emplace_back(&UploadTester::worker, this);
    }

    // Allow test to run
    this_thread::sleep_for(seconds(config.test_duration_seconds));
    stop_requested = true;

    cout << "Stopping uploads...\n";

    // Wait for workers
    for (auto& worker : workers) {
        worker.join();
    }
    monitor.join();

    auto total_end = high_resolution_clock::now();

    // Aggregate calculations
    double total_seconds = duration_cast<duration<double>>(total_end - total_start).count();
    long long bytes = total_uploaded_bytes.load();
    double total_megabits = (bytes * 8.0) / 1e6;
    double aggregate_speed = total_megabits / total_seconds;

    // Populate structured result
    result.uploaded_mb = total_uploaded_bytes / (1024.0 * 1024.0);
    result.wall_time_seconds = total_seconds;
    result.aggregate_speed_mbps = aggregate_speed;
    result.min_speed = sampler.minimum();
    result.max_speed = sampler.maximum();
    result.running_average_speed = sampler.running_average();
    result.standard_deviation = sampler.stddev();
    result.p5 = sampler.percentile(0.05);
    result.p50 = sampler.percentile(0.50);
    result.p95 = sampler.percentile(0.95);
    result.p99 = sampler.percentile(0.99);

    return result;
}