#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "LogParser.h"
#include "StatsManager.h"
#include "LogEntry.h"

// Day 2 goal: Parse the whole file sequentially and verify the data.
// No threads yet.

int main(int argc, char** argv) {
    // --- 1) Validate CLI input ---
    if (argc < 2) {
        std::cerr << "Usage: ./LogAnalyzer <log_file_path>\n";
        return 1;
    }

    // --- 2) Read the file into memory (one line per entry) ---
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string> lines;
    lines.reserve(1'000); // small reserve; grows if needed
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    file.close();

    if (lines.empty()) {
        std::cerr << "Warning: input file is empty.\n";
    }

    // --- 3) Parse all lines into LogEntry objects (sequential) ---
    std::vector<LogEntry> entries = LogParser::parseChunk(lines);

    // --- 4) (Optional) print a few parsed records for visual confirmation ---
    std::cout << "Parsed " << entries.size() << " entries.\n";
    size_t preview = std::min<size_t>(entries.size(), 5);
    for (size_t i = 0; i < preview; ++i) {
        const auto& e = entries[i];
        std::cout << "Level: " << e.level
                  << " | UserID: " << e.userId
                  << " | Latency: " << e.latency << " ms"
                  << " | Status: " << e.statusCode << "\n";
    }

    // --- 5) Update and print simple stats (still sequential) ---
    StatsManager stats;
    stats.update(entries);
    std::cout << "\n=== Summary ===\n";
    stats.printSummary();

    std::cout << "\n✅ Day 2: parsing & basic stats complete.\n";
    return 0;
}
