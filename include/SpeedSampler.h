#pragma once

#include <algorithm>
#include <cmath>
#include <deque>
#include <mutex>
#include <vector>

class SpeedSampler {
   public:
    void reset();

    void add_sample(double mbps);

    double smoothed() const;
    double running_average() const;

    double minimum() const;
    double maximum() const;

    double stddev() const;

    double percentile(double p) const;

   private:
    mutable std::mutex mutex_;
    std::deque<double> window;
    std::vector<double> samples;

    const size_t WINDOW_SIZE = 5;
    const int WARMUP = 3;

    int count = 0;
    double sum = 0;

    double min_v = 0;
    double max_v = 0;
};