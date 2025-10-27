#pragma once
#include <mutex>
#include <iostream>

class StatsManager {
public:
    void update(const std::vector<LogEntry>& entries);
    void merge(const StatsManager& other);
    void printSummary() const;

private:
    mutable std::mutex mtx;
    size_t totalLogs = 0;
    size_t errorCount = 0;
    double totalLatency = 0.0;
};
