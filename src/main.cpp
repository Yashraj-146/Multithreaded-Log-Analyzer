#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <future>
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
        std::cerr << "Error: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    file.close();

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

        // enqueue returns a future we can wait on
        futures.emplace_back(pool.enqueue([chunk, &stats]() {
            auto entries = LogParser::parseChunk(chunk);
            stats.update(entries);
        }));
    }

    // ✅ Explicitly wait for all threads to complete
    for (auto& f : futures) f.get();

    std::cout << "\n=== Final Summary ===\n";
    stats.printSummary();

    std::cout << "\n✅ Multithreading complete and synchronized deterministically.\n";
    return 0;
}