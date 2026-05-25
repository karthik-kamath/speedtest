#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "SpeedTestOrchestrator.h"
#include "httplib.h"

using namespace httplib;

int main() {
    Server svr;
    SpeedTestOrchestrator orchestrator;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::ifstream file("assets/index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "text/html");
    });

    // -----------------------------
    // /speedtest endpoint
    // -----------------------------
    svr.Get("/speedtest", [&](const Request& req, Response& res) {
        std::cout << "Speedtest request received...\n";

        SpeedTestConfig config;

        // default config (or parse query params later)
        config.download_url = "https://proof.ovh.net/files/1Gb.dat";
        config.upload_url = "https://httpbin.org/post";
        config.thread_count = 4;
        config.test_duration_seconds = 10;
        config.upload_payload_mb = 50;
        config.download_payload_mb = 100;

        // Run your full system
        UnifiedSpeedTestResult result = orchestrator.run(config);

        // Convert to JSON manually
        std::string json = "{";

        json += "\"download\": {";
        json += "\"mbps\": " + std::to_string(result.download_result.aggregate_speed_mbps) + ",";
        json += "\"mb_transferred\": " + std::to_string(result.download_result.downloaded_mb);
        json += "},";

        json += "\"upload\": {";
        json += "\"mbps\": " + std::to_string(result.upload_result.aggregate_speed_mbps) + ",";
        json += "\"mb_transferred\": " + std::to_string(result.upload_result.uploaded_mb);
        json += "}";

        json += "}";

        res.set_content(json, "application/json");
    });

    std::cout << "Server running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}