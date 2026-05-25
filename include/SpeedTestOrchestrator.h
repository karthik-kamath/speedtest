#ifndef SPEED_TEST_ORCHESTRATOR_H
#define SPEED_TEST_ORCHESTRATOR_H

#include "DownloadTester.h"
#include "UnifiedSpeedTestResult.h"
#include "UploadTester.h"

class SpeedTestOrchestrator {
   public:
    UnifiedSpeedTestResult run(const SpeedTestConfig& config);

   private:
    DownloadTester download_tester;
    UploadTester upload_tester;
};

#endif