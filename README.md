# SpeedTest

A multi-threaded internet speed testing application written in modern C++ using:

- libcurl
- cpp-httplib
- CMake
- vcpkg

The project supports:

- Parallel download testing
- Parallel upload testing
- Real-time telemetry
- Statistical analysis
- HTTP API
- Browser-based frontend

# Features

- Multi-threaded download/upload workers
- Configurable test duration
- Configurable thread count
- Aggregate Mbps calculation
- Running average speed
- Min / Max speed
- Standard deviation
- Percentile metrics (P5/P50/P95/P99)
- Lightweight web frontend
- JSON HTTP API

# Tech Stack

- C++17
- libcurl
- cpp-httplib
- CMake
- vcpkg

# Project Structure

speedtest/
│
├── assets/
│   └── index.html
│
├── include/
│   ├── DownloadTester.h
│   ├── UploadTester.h
│   ├── SpeedMonitor.h
│   ├── SpeedSampler.h
│   ├── SpeedTestConfig.h
│   ├── SpeedTestOrchestrator.h
│   ├── ResultPrinter.h
│   └── UnifiedSpeedTestResult.h
│
├── src/
│   ├── main.cpp
│   ├── DownloadTester.cpp
│   ├── UploadTester.cpp
│   ├── SpeedMonitor.cpp
│   ├── SpeedSampler.cpp
│   ├── SpeedTestOrchestrator.cpp
│   ├── ResultPrinter.cpp
│   └── SpeedTestServer.cpp
│
├── third_party/
│   └── httplib.h
│
├── CMakeLists.txt
├── vcpkg.json
└── README.md

# Prerequisites

Install:

* Git
* CMake
* C++ compiler
* vcpkg

Recommended compiler:

* MSVC (Visual Studio Build Tools)
* MinGW-w64

# Installing vcpkg

Clone vcpkg:
git clone https://github.com/microsoft/vcpkg.git

Go into the folder:
cd vcpkg

Bootstrap vcpkg:
### Windows
.\bootstrap-vcpkg.bat

### Linux/macOS
./bootstrap-vcpkg.sh

# Installing Dependencies

Install libcurl using vcpkg:
.\vcpkg install curl

# Building The Project

Clone the repository:
git clone https://github.com/YOUR_USERNAME/speedtest.git

Go into the project:
cd speedtest

Generate build files:
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

Build:
cmake --build build

# Running CLI Version

Run:
.\build\Debug\speedtest_cli.exe

# Example output:

========== DOWNLOAD ==========
Downloaded MB: 64.18
Aggregate Speed: 51.41 Mbps

========== UPLOAD ==========
Uploaded MB: 40.56
Aggregate Speed: 33.16 Mbps


# Running Web Version

Run:
.\build\Debug\speedtest_server.exe

Open browser:
http://localhost:8080

# Example API Response

{
  "download": {
    "mbps": 68.36,
    "mb_transferred": 83.37
  },
  "upload": {
    "mbps": 42.09,
    "mb_transferred": 51.31
  }
}

# Notes

* The project currently uses public test endpoints.
* Download usage limits are configurable.
* The frontend is intentionally lightweight and framework-free.

# Future Improvements

* Latency / jitter measurement
* HTTPS support
* WebSocket live telemetry
* Historical result storage
* Geographic server selection
* Docker deployment

# License

All Rights Reserved © 2026 KARTHIK KAMATH K
This project is provided for viewing and educational purposes only.
You may not copy, modify, distribute, sublicense, or use this software without explicit permission from the author.
