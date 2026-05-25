#include "SpeedTestOrchestrator.h"

#include <thread>

UnifiedSpeedTestResult SpeedTestOrchestrator::run(const SpeedTestConfig& config) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    UnifiedSpeedTestResult result;
    result.download_result = download_tester.run(config);
    std::this_thread::sleep_for(std::chrono::seconds(config.cooldown));
    result.upload_result = upload_tester.run(config);

    curl_global_cleanup();
    return result;
}