#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include <algorithm>

#include "LogParser.h"
#include "StatsManager.h"
#include "ThreadPool.h"

// --- Utility to measure time of any callable ---
inline double measureExecutionTime(std::function<void()> func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

inline double nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./LogAnalyzer <log_file_path>\n";
        return 1;
    }

    // --- Read log file ---
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) lines.push_back(line);
    file.close();

    std::cout << "Loaded " << lines.size() << " log entries.\n";

    // --- Sequential benchmark ---
    double sequentialTime = measureExecutionTime([&]() {
        auto entries = LogParser::parseChunk(lines);
        StatsManager stats;
        stats.update(entries);
    });
    std::cout << "Sequential Processing Time: " << sequentialTime << " sec\n";

    // --- Multithreaded benchmark with local aggregation ---
    double parallelTime = measureExecutionTime([&]() {
        size_t numThreads = std::max<size_t>(1, std::thread::hardware_concurrency());
        size_t chunkSize = std::max<size_t>(1000, lines.size() / (numThreads * 2));

        ThreadPool pool(numThreads);
        StatsManager globalStats;
        std::vector<std::future<void>> futures;

        double startMs = nowMs();

        for (size_t i = 0; i < numThreads; ++i) {
            size_t startIdx = i * chunkSize;
            if (startIdx >= lines.size()) break;
            size_t endIdx = std::min(lines.size(), startIdx + chunkSize);

            std::vector<std::string> chunk(lines.begin() + startIdx, lines.begin() + endIdx);

            futures.emplace_back(pool.enqueue([chunk, &globalStats]() {
                auto entries = LogParser::parseChunk(chunk);

                // Local accumulation
                StatsManager local;
                local.update(entries);

                // Single lock merge
                globalStats.merge(local);
            }));
        }

        for (auto& f : futures) f.get();

        double endMs = nowMs();
        std::cout << "Parallel work finished in " << (endMs - startMs) << " ms\n";

        std::cout << "\n=== Final Summary (Parallel) ===\n";
        globalStats.printSummary();
    });

    std::cout << "Multithreaded Processing Time: " << parallelTime << " sec\n";

    double speedup = sequentialTime / parallelTime;
    std::cout << "\nSpeedup: x" << speedup << "\n";
    std::cout << "✅ Day 5 Complete — Optimized & Profiled Successfully!\n";

    return 0;
}