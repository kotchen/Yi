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

template <class F, class... Args>
auto yi::LockFreeThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    while (true)
    {
        if (tasks.Push([task]()
                       { (*task)(); }))
            break;
        else
            condition.notify_all();
    }
    condition.notify_one();
    return res;
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