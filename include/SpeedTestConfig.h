#ifndef SPEED_TEST_CONFIG_H
#define SPEED_TEST_CONFIG_H

#include <string>

struct SpeedTestConfig {
    std::string download_url;
    std::string upload_url;

    int thread_count = 4;
    int test_duration_seconds = 10;

    size_t upload_payload_mb = 50;
    size_t download_payload_mb = 50;
    std::chrono::seconds cooldown = std::chrono::seconds(2);
};

#endif