#include "StatsManager.h"
#include "LogEntry.h"
#include <iostream>
#include <mutex>

void StatsManager::update(const std::vector<LogEntry>& entries) {
    for (const auto& e : entries) {
        ++totalLogs;
        if (e.statusCode >= 400)
            ++errorCount;
        totalLatency += e.latency;
    }
}

void StatsManager::merge(const StatsManager& other) {
    std::lock_guard<std::mutex> lock(mtx);
    totalLogs += other.totalLogs;
    errorCount += other.errorCount;
    totalLatency += other.totalLatency;
}

void StatsManager::printSummary() const {
    std::lock_guard<std::mutex> lock(mtx);
    if (totalLogs == 0) {
        std::cout << "No logs processed.\n";
        return;
    }
    std::cout << "Processed Logs: " << totalLogs << "\n"
              << "Average Latency: " << (totalLatency / totalLogs) << " ms\n"
              << "Error Rate: " << (100.0 * errorCount / totalLogs) << " %\n";
}
