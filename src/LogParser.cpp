#include "LogParser.h"
#include <sstream>
#include <algorithm>
#include <cctype>

static inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}

std::vector<LogEntry> LogParser::parseChunk(const std::vector<std::string>& lines) {
    std::vector<LogEntry> entries;
    entries.reserve(lines.size());

    for (const auto& line : lines) {
        std::istringstream ss(line);
        std::string token;

        LogEntry e;
        std::string date, time, level;

        // Parse date, time, and level
        if (!std::getline(ss, date, ' ')) continue;
        if (!std::getline(ss, time, ',')) continue;
        if (!std::getline(ss, level, ',')) continue;
        trim(level);
        e.level = level;

        // Remaining tokens separated by commas
        while (std::getline(ss, token, ',')) {
            trim(token);
            size_t eq = token.find('=');
            if (eq == std::string::npos) continue;

            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);
            trim(key); trim(val);

            if (key == "user_id") e.userId = std::stoi(val);
            else if (key == "latency") {
                if (val.find("ms") != std::string::npos)
                    val = val.substr(0, val.find("ms"));
                e.latency = std::stod(val);
            }
            else if (key == "status") e.statusCode = std::stoi(val);
        }

        entries.push_back(e);
    }

    return entries;
}