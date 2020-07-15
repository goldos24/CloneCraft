#pragma once
#include <vector>
#include <functional>
#include <thread>
#include <mutex>

struct ThreadPool
{
    bool isRunning = true;

    struct ThreadContainer
    {
        std::thread thread;
        ThreadPool& pool;

        ThreadContainer(ThreadPool* pool);
    };

    std::mutex fQueueMutex;
    const int threadCount;
    std::vector<ThreadContainer> threads;
    std::vector<std::function<void(void)>> fQueue;

    std::function<void()> getFunction();

    ThreadPool(const ThreadPool&) = delete;

    ThreadPool();

    ThreadPool(int threadCount);

    void runAsynchronously(std::function<void()> threadFunc);

    ~ThreadPool();

private:
    void initThreads();
};

