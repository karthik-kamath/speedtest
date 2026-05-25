#include "SpeedSampler.h"

void SpeedSampler::reset() {
    window.clear();
    samples.clear();
    count = 0;
    sum = 0;
    min_v = 0;
    max_v = 0;
}

void SpeedSampler::add_sample(double v) {
    std::lock_guard<std::mutex> lock(mutex_);
    count++;

    if (count <= WARMUP) return;
    samples.push_back(v);

    if (samples.size() == 1)
        min_v = v;
    else
        min_v = std::min(min_v, v);

    max_v = std::max(max_v, v);
    sum += v;
    window.push_back(v);
    if (window.size() > WINDOW_SIZE) window.pop_front();
}

double SpeedSampler::smoothed() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (window.empty()) return 0;
    double s = 0;
    for (double v : window) s += v;
    return s / window.size();
}

double SpeedSampler::running_average() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (samples.empty()) return 0;
    return sum / samples.size();
}

double SpeedSampler::minimum() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return min_v;
}

double SpeedSampler::maximum() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return max_v;
}

double SpeedSampler::stddev() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (samples.empty()) return 0;
    double mean = sum / samples.size();
    double v = 0;
    for (double x : samples) v += (x - mean) * (x - mean);
    return std::sqrt(v / samples.size());
}

double SpeedSampler::percentile(double p) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (samples.empty()) return 0;
    auto s = samples;
    std::sort(s.begin(), s.end());
    size_t idx = static_cast<size_t>(p * (s.size() - 1));
    if (idx >= s.size()) idx = s.size() - 1;
    return s[idx];
}