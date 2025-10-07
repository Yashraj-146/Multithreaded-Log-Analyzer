#pragma once
#include <string>
#include <chrono>

struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string level;
    int userId;
    double latency;
    int statusCode;
};
