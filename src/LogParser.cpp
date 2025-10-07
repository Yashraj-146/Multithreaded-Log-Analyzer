#include "LogParser.h"
#include <sstream>
#include <string>
#include <vector>

std::vector<LogEntry> LogParser::parseChunk(const std::vector<std::string>& lines) {
    std::vector<LogEntry> entries;
    for (const auto& line : lines) {
        std::istringstream ss(line);
        std::string date, time, level, token;

        LogEntry e;

        if (!std::getline(ss, date, ' ')) continue;
        if (!std::getline(ss, time, ',')) continue;
        if (!std::getline(ss, level, ',')) continue;

        // Now parse remaining tokens
        while (std::getline(ss, token, ',')) {
            size_t eq = token.find('=');
            if (eq != std::string::npos) {
                std::string key = token.substr(0, eq);
                std::string val = token.substr(eq + 1);

                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                val.erase(0, val.find_first_not_of(" \t"));
                val.erase(val.find_last_not_of(" \t") + 1);

                if (key == "user_id") e.userId = std::stoi(val);
                else if (key == "latency") {
                    if (val.find("ms") != std::string::npos)
                        val = val.substr(0, val.find("ms"));
                    e.latency = std::stod(val);
                }
                else if (key == "status") e.statusCode = std::stoi(val);
            }
        }
        entries.push_back(e);
    }
    return entries;
}
