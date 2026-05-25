#ifndef UNIFIED_SPEED_TEST_RESULT_H
#define UNIFIED_SPEED_TEST_RESULT_H

#include "DownloadTester.h"
#include "UploadTester.h"

struct UnifiedSpeedTestResult {
    DownloadResult download_result;
    UploadResult upload_result;
};

#endif