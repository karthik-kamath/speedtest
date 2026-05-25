# SpeedTest

A multi-threaded internet speed testing application written in modern C++ using:

- libcurl
- cpp-httplib
- CMake
- vcpkg

The project includes:

- Parallel download testing
- Parallel upload testing
- Real-time telemetry
- Statistical analysis
- HTTP API
- Browser-based frontend

---

# Features

- Multi-threaded download/upload workers
- Configurable test duration
- Configurable thread count
- Aggregate Mbps calculation
- Running average speed
- Min / Max speed tracking
- Standard deviation calculation
- Percentile metrics (P5 / P50 / P95 / P99)
- Lightweight web frontend
- JSON HTTP API
- CLI and browser-based execution modes

---

# Tech Stack

- C++17
- libcurl
- cpp-httplib
- CMake
- vcpkg

---

# Project Structure

```text
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
```

---

# Prerequisites

Install the following tools:

- Git
- CMake
- A C++ compiler
- vcpkg

Recommended compilers:

- MSVC (Visual Studio Build Tools)
- MinGW-w64

---

# Installing vcpkg

Clone vcpkg:

```bash
git clone https://github.com/microsoft/vcpkg.git
```

Go into the folder:

```bash
cd vcpkg
```

Bootstrap vcpkg:

### Windows

```bash
.\bootstrap-vcpkg.bat
```

### Linux/macOS

```bash
./bootstrap-vcpkg.sh
```

---

# Installing Dependencies

Install libcurl using vcpkg:

```bash
.\vcpkg install curl
```

---

# Cloning The Repository

```bash
git clone https://github.com/karthik-kamath/speedtest.git
```

Go into the project folder:

```bash
cd speedtest
```

---

# Building The Project

Generate build files:

```bash
cmake -S . -B build ^
-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build the project:

```bash
cmake --build build
```

---

# Running CLI Version

Run:

```bash
.\build\Debug\speedtest_cli.exe
```

Example output:

```text
========== DOWNLOAD ==========
Downloaded MB: 64.18
Aggregate Speed: 51.41 Mbps

========== UPLOAD ==========
Uploaded MB: 40.56
Aggregate Speed: 33.16 Mbps
```

---

# Running Web Version

Run:

```bash
.\build\Debug\speedtest_server.exe
```

Open browser:

```text
http://localhost:8080
```

---

# Example API Response

```json
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
```

---

# Notes

- The project currently uses public test endpoints.
- Download usage limits are configurable.
- The frontend is intentionally lightweight and framework-free.
- Public test endpoints may produce different results compared to commercial speed testing platforms.

---

# Future Improvements

- Latency / jitter measurement
- HTTPS support
- WebSocket live telemetry
- Historical result storage
- Geographic server selection
- Docker deployment

---

# License

All Rights Reserved © 2026 KARTHIK KAMATH K

This repository is provided for viewing and educational purposes only.

You may not copy, modify, distribute, sublicense, or use this software without explicit permission from the author.
