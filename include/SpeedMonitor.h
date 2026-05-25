#pragma once

#include <atomic>
#include <thread>

#include "SpeedSampler.h"

class SpeedMonitor {
   public:
    void start(std::atomic<long long>* byte_counter, std::atomic<bool>* stop_flag,
               SpeedSampler* sampler);
    void join();

   private:
    void run();
    std::atomic<long long>* bytes = nullptr;
    std::atomic<bool>* stop = nullptr;
    SpeedSampler* sampler = nullptr;
    std::thread monitor_thread;
};