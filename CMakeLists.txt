cmake_minimum_required(VERSION 3.10)
project(MultithreadedLogAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Main executable
add_executable(LogAnalyzer
    src/main.cpp
    src/LogParser.cpp
    src/StatsManager.cpp
    src/ThreadPool.cpp
)

# Link threads for multithreading
find_package(Threads REQUIRED)       # Threads library detected automatically -> Needed for multithreading later.
target_link_libraries(LogAnalyzer PRIVATE Threads::Threads)

# Test executable (do NOT include main.cpp here!)
add_executable(runTests
    tests/test_basic.cpp
    src/LogParser.cpp
    src/StatsManager.cpp
    src/ThreadPool.cpp
)

target_link_libraries(runTests PRIVATE Threads::Threads)

enable_testing()
add_test(NAME BasicTests COMMAND runTests)
