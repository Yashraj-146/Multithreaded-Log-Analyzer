# Multithreaded Log Analyzer (C++17)

A high-performance, multithreaded C++17 tool for parsing and analyzing large server/network log files.

## Features
- Parses log files and computes key metrics:
  - Total requests
  - Error rate
  - Average latency
- Multithreaded processing using a custom thread pool
- Efficient memory management with RAII
- Modular CMake-based architecture

## Build
```bash
mkdir build && cd build
cmake ..
make
```
## To Rebuild
cd build
make -j$(sysctl -n hw.ncpu)

## RUN
Single Thread:
./LogAnalyzer ../data/sample.log
Multithread:
./LogAnalyzer ../data/large_sample.log

## Example Output
[PROFILE] File reading: 212 ms
Loaded 100000 log entries.
[PROFILE] Sequential processing: 0.476 sec
[INFO] Using 8 threads, chunk size: 6250
[PROFILE] Thread pool initialization: 0 ms
[PROFILE] Task scheduling: 7 ms
[PROFILE] Thread join/wait: 52 ms
[PROFILE] Total parallel phase: 0.059 sec

=== Final Summary (Parallel) ===
Processed Logs: 50000
Average Latency: 254.702 ms
Error Rate: 50.198 %
[PROFILE] Printing summary: 0 ms

Sequential: 0.476 sec
Parallel:   0.059 sec
Speedup: x8.0678
Completed successfully!

## Author
Yashraj Umesh Panhalkar — aspiring C++ developer passionate about systems and data engineering.

## To push on GitHub
git add . && git commit -m "update" && git push origin main --force
