#pragma once
#include "LogEntry.h"
#include <mutex>
#include <vector>

class StatsManager {
public:
    void update(const std::vector<LogEntry>& entries);
    void printSummary() const;

private:
    mutable std::mutex mtx;
    int totalCount = 0;
    int errorCount = 0;
    double totalLatency = 0.0;
};
