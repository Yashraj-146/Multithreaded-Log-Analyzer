#include "LogParser.h"
#include <cassert>
#include <iostream>

int main() {
    std::vector<std::string> lines = {
        "2025-10-07 13:22:45, INFO, user_id=123, latency=42ms, status=200",
        "2025-10-07 13:22:46, ERROR, user_id=999, latency=120ms, status=500"
    };

    auto entries = LogParser::parseChunk(lines);
    assert(entries.size() == 2);
    assert(entries[0].statusCode == 200);
    assert(entries[1].latency == 120);

    std::cout << "✅ Basic test passed.\n";
    return 0;
}
