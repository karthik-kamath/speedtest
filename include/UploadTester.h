#ifndef UPLOAD_TESTER_H
#define UPLOAD_TESTER_H

#include <curl/curl.h>

#include <atomic>
#include <cfloat>
#include <deque>
#include <mutex>
#include <vector>

#include "SpeedMonitor.h"
#include "SpeedSampler.h"
#include "SpeedTestConfig.h"

class UploadTester;

struct UploadTelemetry {
    double instantaneous_speed = 0;
    double smoothed_speed = 0;
    double running_average = 0;
};

struct UploadResult {
    double uploaded_mb = 0;
    double wall_time_seconds = 0;
    double aggregate_speed_mbps = 0;

    double min_speed = 0;
    double max_speed = 0;

    double smoothed_speed = 0;
    double running_average_speed = 0;

    double standard_deviation = 0;

    double p5 = 0;
    double p50 = 0;
    double p95 = 0;
    double p99 = 0;
};

struct UploadContext {
    std::vector<char> buffer;
    size_t offset = 0;
    size_t total_size = 0;
    UploadTester* tester = nullptr;
};

class UploadTester {
   public:
    UploadResult run(const SpeedTestConfig& config);

   private:
    void worker();

    static size_t read_callback(char* ptr, size_t size, size_t nmemb, void* userdata);

    static int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                 curl_off_t ultotal, curl_off_t ulnow);

    static constexpr int THREAD_COUNT = 4;
    static constexpr int TEST_DURATION_SECONDS = 10;

    std::atomic<bool> stop_requested{false};
    std::atomic<long long> total_uploaded_bytes{0};
    std::mutex console_mutex;

    SpeedTestConfig config;
    SpeedSampler sampler;
    SpeedMonitor monitor;
};

#endif