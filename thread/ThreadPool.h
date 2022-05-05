﻿// 实现一个线程池

#ifndef THREAD_THREADPOOL_H
#define THREAD_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

namespace thread
{

    class ThreadPool
    {
    public:
        ThreadPool(size_t);
        ~ThreadPool();
        template <class F, class... Args>
        auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
        {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);

                // don't allow enqueueing after stopping the pool
                if (stop)
                    throw std::runtime_error("enqueue on stopped ThreadPool");

                tasks.emplace([task]()
                              { (*task)(); });
            }
            condition.notify_one();
            return res;
        }
        void join();

    private:
        // need to keep track of threads so we can join them
        std::vector<std::thread> workers;
        // the task queue
        std::queue<std::function<void()>> tasks;
        // synchronization
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };

}

#endif