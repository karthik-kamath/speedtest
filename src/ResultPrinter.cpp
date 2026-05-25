#include "ResultPrinter.h"

#include <iostream>

using namespace std;

void ResultPrinter::print(const UnifiedSpeedTestResult& result) {
    cout << "\n========== DOWNLOAD ==========\n";
    cout << "Downloaded MB: " << result.download_result.downloaded_mb << "\n";
    cout << "Wall Time: " << result.download_result.wall_time_seconds << " seconds\n";
    cout << "Aggregate Speed: " << result.download_result.aggregate_speed_mbps << " Mbps\n";

    cout << "\n========== UPLOAD ==========\n";
    cout << "Uploaded MB: " << result.upload_result.uploaded_mb << "\n";
    cout << "Wall Time: " << result.upload_result.wall_time_seconds << " seconds\n";
    cout << "Aggregate Speed: " << result.upload_result.aggregate_speed_mbps << " Mbps\n";
    cout << "Min Speed: " << result.upload_result.min_speed << " Mbps\n";
    cout << "Max Speed: " << result.upload_result.max_speed << " Mbps\n";
    cout << "Running Average: " << result.upload_result.running_average_speed << " Mbps\n";
    cout << "Standard Deviation: " << result.upload_result.standard_deviation << " Mbps\n";
    cout << "P5: " << result.upload_result.p5 << " Mbps\n";
    cout << "P50: " << result.upload_result.p50 << " Mbps\n";
    cout << "P95: " << result.upload_result.p95 << " Mbps\n";
    cout << "P99: " << result.upload_result.p99 << " Mbps\n";
}