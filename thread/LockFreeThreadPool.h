#ifndef __YI_LOCKFREE_THREADPOOL__
#define __YI_LOCKFREE_THREADPOOL__
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>
#include <functional>
#include <future>
#include <memory>
#include <utility>
#include <stdexcept>
#include "util/LockFreeQueue.h"

namespace yi
{
    class LockFreeThreadPool
    {

    public:
        LockFreeThreadPool(size_t, size_t);
        template <class F, class... Args>
        auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
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
        ~LockFreeThreadPool();
        void join();

    private:
        // need to keep track of threads so we can join them
        std::vector<std::thread> workers;
        // the task queue
        yi::LockFreeQueue<std::function<void()>> tasks;
        // synchronization
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };

}

#endif