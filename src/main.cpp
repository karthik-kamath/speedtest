#include "ResultPrinter.h"
#include "SpeedTestConfig.h"
#include "SpeedTestOrchestrator.h"

int main() {
    SpeedTestConfig config;

    config.download_url = "https://proof.ovh.net/files/1Gb.dat";
    config.upload_url = "https://httpbin.org/post";
    config.thread_count = 4;
    config.test_duration_seconds = 10;
    config.upload_payload_mb = 50;
    config.download_payload_mb = 100;

    SpeedTestOrchestrator orchestrator;
    UnifiedSpeedTestResult result = orchestrator.run(config);
    ResultPrinter::print(result);

    return 0;
}