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

// Utility: Measure wall-clock time of any callable
inline double measureExecutionTime(std::function<void()> func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main(int argc, char** argv) {
    // --- 1. Validate CLI input ---
    if (argc < 2) {
        std::cerr << "Usage: ./LogAnalyzer <log_file_path>\n";
        return 1;
    }

    // --- 2. Read file into memory ---
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

    if (lines.empty()) {
        std::cerr << "File is empty or unreadable.\n";
        return 1;
    }

    std::cout << "Loaded " << lines.size() << " log entries.\n";
    std::cout << "\n=== Benchmarking Mode ===\n";

    // --- 3. Sequential benchmark ---
    double sequentialTime = measureExecutionTime([&]() {
        auto entries = LogParser::parseChunk(lines);
        StatsManager stats;
        stats.update(entries);
    });
    std::cout << "Sequential Processing Time: " << sequentialTime << " sec\n";

    // --- 4. Multithreaded benchmark ---
    double parallelTime = measureExecutionTime([&]() {
        size_t numThreads = std::max<size_t>(1, std::thread::hardware_concurrency());
        size_t chunkSize = (lines.size() + numThreads - 1) / numThreads;

        ThreadPool pool(numThreads);
        StatsManager stats;
        std::vector<std::future<void>> futures;

        for (size_t i = 0; i < numThreads; ++i) {
            size_t startIdx = i * chunkSize;
            if (startIdx >= lines.size()) break;
            size_t endIdx = std::min(lines.size(), startIdx + chunkSize);

            std::vector<std::string> chunk(lines.begin() + startIdx, lines.begin() + endIdx);

            futures.emplace_back(pool.enqueue([chunk, &stats]() {
                auto entries = LogParser::parseChunk(chunk);
                stats.update(entries);
            }));
        }

        // Wait for all worker tasks to finish
        for (auto& f : futures) f.get();

        std::cout << "\n=== Final Summary (Parallel) ===\n";
        stats.printSummary();
    });

    std::cout << "Multithreaded Processing Time: " << parallelTime << " sec\n";

    // --- 5. Report performance gain ---
    double speedup = sequentialTime / parallelTime;
    std::cout << "\nSpeedup: x" << speedup << "\n";

    std::cout << "\n✅ Day 4 Complete — Benchmarking Successful!\n";
    return 0;
}
