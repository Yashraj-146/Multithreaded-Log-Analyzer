#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>

int main() {
    std::ofstream out("large_sample.log");
    if (!out.is_open()) {
        std::cerr << "Error: could not open output file.\n";
        return 1;
    }

    // Random generators for user_id, latency, status, and level
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> latencyDist(10, 500);
    std::uniform_int_distribution<int> statusDist(200, 599);
    std::uniform_int_distribution<int> userDist(1, 5000);
    std::uniform_int_distribution<int> levelDist(0, 2);

    const std::string levels[] = {"INFO", "WARNING", "ERROR"};

    // Starting timestamp (2025-10-07 13:22:45)
    std::tm tm = {};
    tm.tm_year = 125; // years since 1900 -> 2025
    tm.tm_mon = 9;    // October (0-based)
    tm.tm_mday = 7;
    tm.tm_hour = 13;
    tm.tm_min = 22;
    tm.tm_sec = 45;
    std::time_t baseTime = std::mktime(&tm);

    const int numEntries = 100000;

    for (int i = 0; i < numEntries; ++i) {
        std::time_t currentTime = baseTime + i; // 1-second increments
        std::tm* now = std::localtime(&currentTime);
        std::ostringstream timestamp;
        timestamp << std::put_time(now, "%Y-%m-%d %H:%M:%S");

        int latency = latencyDist(rng);
        int status = statusDist(rng);
        std::string level = levels[levelDist(rng)];
        int userId = userDist(rng);

        out << timestamp.str() << ", " << level
            << ", user_id=" << userId
            << ", latency=" << latency << "ms"
            << ", status=" << status << "\n";
    }

    out.close();
    std::cout << "✅ Generated " << numEntries << " logs in large_sample.log\n";
    return 0;
}
