﻿#include "ThreadPool.h"

// the constructor just launches some amount of workers
inline thread::ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    try
                    {
                        task();
                    }
                    catch(std::exception& e)
                    {
                        std::cout << "running task, with exception" << e.what() << std::endl;
                    }
                }
            }
        );
}


// the destructor joins all threads
inline thread::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
    {
        if (worker.joinable())
            worker.join();
    }
}

inline void thread::ThreadPool::join()
{
    // this->stop = true;
    for (std::thread &worker : workers)
    {
        if (worker.joinable())
            worker.join();
    }
}
