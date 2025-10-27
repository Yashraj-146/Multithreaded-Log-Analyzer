#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <functional>
#include <algorithm>
#include <atomic>
#include <iomanip>

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

    // --- [PROFILE] File Reading ---
    double tStartRead = nowMs();
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
    double tEndRead = nowMs();
    std::cout << "[PROFILE] File reading: " << (tEndRead - tStartRead) << " ms\n";
    std::cout << "Loaded " << lines.size() << " log entries.\n";

    // --- [PROFILE] Sequential Benchmark ---
    double tStartSeq = nowMs();
    auto entries = LogParser::parseChunk(lines);
    StatsManager seqStats;
    seqStats.update(entries);
    double tEndSeq = nowMs();
    std::cout << "[PROFILE] Sequential processing: " << (tEndSeq - tStartSeq) / 1000.0 << " sec\n";

    // --- [PROFILE] Parallel Benchmark with Real-time Progress ---
    double tStartParallel = nowMs();
    size_t numThreads = std::max<size_t>(1, std::thread::hardware_concurrency());
    size_t chunkSize = std::max<size_t>(1000, lines.size() / (numThreads * 2));

    std::cout << "[INFO] Using " << numThreads << " threads, chunk size: " << chunkSize << "\n";

    ThreadPool pool(numThreads);
    StatsManager globalStats;
    std::vector<std::future<void>> futures;
    std::atomic<size_t> processedChunks(0);
    size_t totalChunks = numThreads;

    double tStartTasks = nowMs();
    for (size_t i = 0; i < numThreads; ++i) {
        size_t startIdx = i * chunkSize;
        if (startIdx >= lines.size()) break;
        size_t endIdx = std::min(lines.size(), startIdx + chunkSize);

        std::vector<std::string> chunk(lines.begin() + startIdx, lines.begin() + endIdx);

        futures.emplace_back(pool.enqueue([chunk, &globalStats, &processedChunks, totalChunks]() {
            auto entries = LogParser::parseChunk(chunk);
            StatsManager local;
            local.update(entries);
            globalStats.merge(local);

            // Real-time progress update
            size_t done = ++processedChunks;
            double pct = (100.0 * done) / totalChunks;
            std::cout << "\r[Progress] " << std::setw(3) << (int)pct << "% complete" << std::flush;
        }));
    }
    double tEndTasks = nowMs();
    std::cout << "\n[PROFILE] Task scheduling: " << (tEndTasks - tStartTasks) << " ms\n";

    double tStartJoin = nowMs();
    for (auto& f : futures) f.get();
    double tEndJoin = nowMs();
    std::cout << "\r[Progress] 100% complete ✅\n";
    std::cout << "[PROFILE] Thread join/wait: " << (tEndJoin - tStartJoin) << " ms\n";

    double tEndParallel = nowMs();
    std::cout << "[PROFILE] Total parallel phase: " << (tEndParallel - tStartParallel) / 1000.0 << " sec\n";

    // --- [PROFILE] Final Merge + Summary ---
    double tStartPrint = nowMs();
    std::cout << "\n=== Final Summary (Parallel) ===\n";
    globalStats.printSummary();
    double tEndPrint = nowMs();
    std::cout << "[PROFILE] Printing summary: " << (tEndPrint - tStartPrint) << " ms\n";

    // --- [PROFILE] Speedup Calculation & CSV Logging ---
    double sequentialTime = (tEndSeq - tStartSeq) / 1000.0;
    double parallelTime = (tEndParallel - tStartParallel) / 1000.0;
    double speedup = sequentialTime / parallelTime;

    std::cout << "\nSequential: " << sequentialTime << " sec\n";
    std::cout << "Parallel:   " << parallelTime << " sec\n";
    std::cout << "Speedup: x" << speedup << "\n";

    // Save benchmark to CSV
    std::ofstream csv("profiling_data.csv", std::ios::app);
    csv << numThreads << "," << sequentialTime << "," << parallelTime << "," << speedup << "\n";
    csv.close();

    std::cout << "✅ Profiling completed — data saved to profiling_data.csv\n";
    return 0;
}