# 🧩 Multithreaded Log Analyzer

A high-performance, multithreaded C++17 application that analyzes large-scale log files, computes performance statistics, and demonstrates real-world concurrency, profiling, and optimization techniques.

# 🚀 Features

🧵 Multithreaded Log Processing — Uses a custom thread pool for concurrent parsing.

📊 Real-Time Progress Bar — Displays live processing progress in the terminal.

⚙️ Profiling Instrumentation — Measures and logs performance at every stage.

📈 Performance Visualization — Exports timing data to CSV and generates Speedup graphs.

🧮 Optimized Stats Aggregation — Thread-local accumulation with minimal locking.

💾 Asynchronous File Reading (optional) — Scales efficiently with large datasets.

# 🧠 Architecture Overview
```bash
├── ThreadPool        → Manages worker threads and tasks
├── LogParser         → Parses log lines into structured entries
├── StatsManager      → Aggregates latency, error rate, and totals
├── profiling_data.csv → Stores runtime benchmarks
└── plot_speedup.py   → Visualizes performance scaling
```

# 🧱 Project Structure
```MultithreadedLogAnalyzer/
├── CMakeLists.txt
├── include/
│   ├── LogEntry.h
│   ├── LogParser.h
│   ├── StatsManager.h
│   ├── ThreadPool.h
├── src/
│   ├── main.cpp
│   ├── LogParser.cpp
│   ├── StatsManager.cpp
│   ├── ThreadPool.cpp
├── data/
│   └── large_sample.log
├── generate_logs.cpp
├── plot_speedup.py
└── profiling_data.csv
```

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
Multithread:
./LogAnalyzer ../data/large_sample.log

## Example Output
```bash
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
```

# 📊 Visualize Performance
Run the Python script to generate a speedup plot:
```bash
python3 plot_speedup.py
```

# 🧩 Example Benchmark Results
Threads	Sequential (s)	Parallel (s)	Speedup (x)

1	5.23	5.23	1.00

2	5.23	2.68	1.95

4	5.23	1.45	3.61

8	5.23	1.19	4.39

## Author
Yashraj Umesh Panhalkar [@Yashraj-146]

💼 Focus: C++ / Systems Programming / Performance Engineering

## To push on GitHub
git add . && git commit -m "update" && git push origin main --force
