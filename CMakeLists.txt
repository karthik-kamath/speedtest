cmake_minimum_required(VERSION 3.16)

project(SpeedTestEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(third_party)

# Source files
add_executable(speedtest_cli
    src/main.cpp
    src/UploadTester.cpp
    src/DownloadTester.cpp
    src/SpeedTestOrchestrator.cpp
    src/ResultPrinter.cpp
    src/SpeedSampler.cpp
    src/SpeedMonitor.cpp
)

add_executable(speedtest_server
    src/SpeedTestServer.cpp
    src/UploadTester.cpp
    src/DownloadTester.cpp
    src/SpeedTestOrchestrator.cpp
    src/ResultPrinter.cpp
    src/SpeedSampler.cpp
    src/SpeedMonitor.cpp
)

find_package(CURL REQUIRED)

target_link_libraries(speedtest_cli PRIVATE CURL::libcurl)
target_link_libraries(speedtest_server PRIVATE CURL::libcurl)