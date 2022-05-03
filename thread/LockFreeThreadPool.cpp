#include "LockFreeThreadPool.h"
yi::LockFreeThreadPool::LockFreeThreadPool(size_t threads, size_t queue_size)
    : stop(false),
      tasks(queue_size)
{
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back([this]
                             {
            for(;;)
            {
                std::function<void()> task;

                auto res = tasks.Pop(task);

                if (res)
                {
                    try
                    {
                        task();
                    }
                    catch (std::exception& e)
                    {
                        std::cout << "running task, with exception" << e.what() <<std::endl;
                    }
                }
                else
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock,
                        [this]{ return stop || !tasks.Empty(); });
                    if (stop && tasks.Empty())
                        return;
                }
                // if (stop && tasks.Empty())
                //     return;
            } });
}

yi::LockFreeThreadPool::~LockFreeThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (auto &worker : workers)
        worker.join();
}


inline void yi::LockFreeThreadPool::join()
{
    this->stop = true;
    for (auto &worker : workers)
    {
        if (worker.joinable())
            worker.join();
    }
}