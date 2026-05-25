#include "SpeedMonitor.h"

#include <chrono>

void SpeedMonitor::start(std::atomic<long long>* byte_counter, std::atomic<bool>* stop_flag,
                         SpeedSampler* sampler_ptr) {
    bytes = byte_counter;
    stop = stop_flag;
    sampler = sampler_ptr;

    monitor_thread = std::thread(&SpeedMonitor::run, this);
}

void SpeedMonitor::join() {
    if (monitor_thread.joinable()) monitor_thread.join();
}

void SpeedMonitor::run() {
    using namespace std::chrono;
    long long previous_bytes = bytes->load();
    auto previous_time = high_resolution_clock::now();

    while (!stop->load()) {
        std::this_thread::sleep_for(milliseconds(500));

        long long current_bytes = bytes->load();
        auto current_time = high_resolution_clock::now();
        double elapsed_seconds =
            duration_cast<duration<double>>(current_time - previous_time).count();
        if (elapsed_seconds <= 0) continue;
        long long delta_bytes = current_bytes - previous_bytes;
        double mbps = (delta_bytes * 8.0) / (elapsed_seconds * 1e6);
        sampler->add_sample(mbps);

        previous_bytes = current_bytes;
        previous_time = current_time;
    }
}