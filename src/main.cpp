#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include "LogParser.h"
#include "StatsManager.h"
#include "ThreadPool.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./LogAnalyzer <log_file_path>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file.\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) lines.push_back(line);
    file.close();

    size_t numThreads = std::thread::hardware_concurrency();
    ThreadPool pool(numThreads);
    StatsManager stats;

    size_t chunkSize = lines.size() / numThreads;
    std::vector<std::future<void>> futures;

    for (size_t i = 0; i < numThreads; ++i) {
        auto start = lines.begin() + i * chunkSize;
        auto end = (i == numThreads - 1) ? lines.end() : start + chunkSize;
        std::vector<std::string> chunk(start, end);

        pool.enqueue([chunk, &stats]() {
            auto entries = LogParser::parseChunk(chunk);
            stats.update(entries);
        });
    }

    // Wait for threads to finish (pool destructor joins them)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stats.printSummary();

    return 0;
}