#pragma once
#include "LogEntry.h"
#include <string>
#include <vector>

class LogParser {
public:
    static std::vector<LogEntry> parseChunk(const std::vector<std::string>& lines);
};
