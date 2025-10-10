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

## RUN
Single Thread:
./LogAnalyzer ../data/sample.log
Multithread:
./LogAnalyzer ../data/large_sample.log

## Example Output
Processed Logs: 100000
Average Latency: 48.7 ms  
Error Rate: 3.5 %

## Author
Yashraj Umesh Panhalkar — aspiring C++ developer passionate about systems and data engineering.

## To push on GitHub
git add . && git commit -m "update" && git push origin main --force