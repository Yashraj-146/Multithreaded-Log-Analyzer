#pragma once
/* It prevents multiple inclusions of the same file, 
which can lead to redefinition errors and other issues. */

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

// The explicit keyword is a safeguard to prevent 
// unintended implicit conversions(like ThreadPool pool = 4;), 
// making the code more robust and easier to understand.

class ThreadPool {
public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};