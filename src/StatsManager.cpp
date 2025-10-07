#include "StatsManager.h"
#include <iostream>

void StatsManager::update(const std::vector<LogEntry>& entries) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& e : entries) {
        totalCount++;
        if (e.statusCode >= 400) errorCount++;
        totalLatency += e.latency;
    }
}

void StatsManager::printSummary() const {
    std::lock_guard<std::mutex> lock(mtx);
    double avgLatency = totalCount ? totalLatency / totalCount : 0.0;
    double errorRate = totalCount ? (100.0 * errorCount / totalCount) : 0.0;

    std::cout << "Processed Logs: " << totalCount << "\n";
    std::cout << "Average Latency: " << avgLatency << " ms\n";
    std::cout << "Error Rate: " << errorRate << " %\n";
}
