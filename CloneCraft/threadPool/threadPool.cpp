#include "threadPool.h"



int processerCount = std::thread::hardware_concurrency();

std::function<void()> ThreadPool::getFunction()
{
start:
    fQueueMutex.lock();
    if (fQueue.size() == 0)
    {
        fQueueMutex.unlock();
        goto start;
    }

    std::function<void()> threadFunction = this->fQueue[0];
    this->fQueue.erase(fQueue.begin());
    fQueueMutex.unlock();
    return threadFunction;

}

ThreadPool::ThreadPool() :
    threadCount(processerCount > 0 ? processerCount : 1)
{
    this->initThreads();
}

ThreadPool::ThreadPool(int threadCount) :
    threadCount(threadCount)
{
    this->initThreads();
}

void ThreadPool::runAsynchronously(std::function<void()> threadFunc)
{
    fQueueMutex.lock();
    this->fQueue.push_back(threadFunc);
    fQueueMutex.unlock();
}

ThreadPool::~ThreadPool()
{
start:
    this->fQueueMutex.lock();
    if (this->fQueue.size() > 0)
    {
        this->fQueueMutex.unlock();
        goto start;
    }
    this->fQueueMutex.unlock();
    this->isRunning = false;
    for (int i = 0; i < this->threadCount; ++i)
    {
        this->threads[i].thread.join();
    }
}

void ThreadPool::initThreads()
{
    for (int i = 0; i < this->threadCount; ++i)
    {
        threads.push_back(ThreadContainer(this));
    }
}

ThreadPool::ThreadContainer::ThreadContainer(ThreadPool* pool) :
    pool(*pool)
{
    new(&this->thread) std::thread([this]()
        {
            while (this->pool.isRunning)
            {
                this->pool.getFunction()();
            }
        }
    );
}

