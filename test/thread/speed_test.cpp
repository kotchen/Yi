#include "util/LockFreeQueue.h"
#include "../Clock.h"
#include <mutex>
#include <condition_variable>
// 测试无锁队列的正确性和性能
// 正确性测试：
// 多线程对队列进行写的操作，然后将数据读出来，看看是否有数据丢失

template<typename T>
class Queue
{
private:
    std::mutex _mutex;
    std::deque<T> _queue;
public:
    void Push(const T &data)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_back(data);
    }

    void Push(T &&data)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_back(std::move(data));
    }

    bool Pop(T &data)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_queue.empty())
            return false;
        data = _queue.front();
        _queue.pop_front();
        return true;
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.empty();
    }
};

void test_lockfree_queue_push()
{
    yi::LockFreeQueue<int> queue(1024);
    std::vector<std::thread> threads;
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue](){
            for (int i = 0 ;i<256; i++)
            {
                queue.Push(i);
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    std::cout << "插入完成" << std::endl;
    std::deque<int> read_queue;
    while (!queue.Empty())
    {
        int data = 0;
        queue.Pop(data);
        read_queue.push_back(data);
    }
    std::cout << "read_queue size: " << read_queue.size() << std::endl;
}

void test_lockfree_queue_pop()
{
    yi::LockFreeQueue<int> queue(1024);
    for (int i = 0; i<1024; i++)
    {
        queue.Push(i);
    }
    std::cout << "插入完成" << std::endl;
    std::vector<std::thread> threads;
    std::atomic<int> read_size(0);
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue, &read_size](){
            while (!queue.Empty())
            {
                int data = 0;
                if(queue.Pop(data))
                    read_size++;
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    std::cout << "读取完成" << std::endl;
    std::cout << "read_queue size: " << read_size << std::endl;
}

// 测试无锁队列的性能
// 读性能测试，先插入数据，然后用多线程读取
void test_lockfree_queue_read_performance()
{
    yi::LockFreeQueue<int> queue(1048576);
    for (int i = 0; i<1048576; i++)
    {
        queue.Push(i);
    }
    std::vector<std::thread> threads;
    yi::Clock clock;
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue](){
            while (!queue.Empty())
            {
                int data = 0;
                queue.Pop(data);
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
}

void test_normal_queue_read_performance()
{
    Queue<int> queue;
    for (int i = 0; i<1048576; i++)
    {
        queue.Push(i);
    }
    // std::cout << "插入完成" << std::endl;
    std::vector<std::thread> threads;
    yi::Clock clock;
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue](){
            while (!queue.Empty())
            {
                int data = 0;
                queue.Pop(data);
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
}

void test_lock_free_queue_write_performance()
{
    yi::LockFreeQueue<int> queue(1048576);
    std::vector<std::thread> threads;
    yi::Clock clock;
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue](){
            for (int i = 0 ;i<1048576/4; i++)
            {
                queue.Push(i);
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    std::cout << "插入完成" << std::endl;
}

void test_normal_queue_write_performance()
{
    Queue<int> queue;
    std::vector<std::thread> threads;
    yi::Clock clock;
    for (int i = 0; i<4; i++)
    {
        threads.emplace_back([&queue](){
            for (int i = 0 ;i<1048576/4; i++)
            {
                queue.Push(i);
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    std::cout << "插入完成" << std::endl;
}

// void test_lock_free_queue_comprehensive_performance()
// {
//     yi::LockFreeQueue<int> queue(1048576);
//     std::vector<std::thread> write_threads;
//     std::vector<std::thread> read_threads;
//     std::atomic<int> count(1048576);
//     yi::Clock clock;
//     for (int i = 0; i<4; i++)
//     {
//         write_threads.emplace_back([&queue](){
//             for (int i = 0 ;i<1048576/4; i++)
//             {
//                 queue.Push(i);
//             }
//         });
//     }

//     for (int i = 0; i<4; i++)
//     {
//         read_threads.emplace_back([&queue, &count](){
//             while (true)
//             {
//                 if (count == 0)
//                     break;
//                 else
//                 {
//                     int data = 0;
//                     if (queue.Pop(data))
//                         count--;
//                 }
//             }
//         });
//     }
//     for (auto &thread : write_threads)
//     {
//         thread.join();
//     }
//     for (auto &thread : read_threads)
//     {
//         thread.join();
//     }
// }

int main(int argc, char const *argv[])
{
    // test_lockfree_queue_push();
    // test_lockfree_queue_pop();
    test_lockfree_queue_read_performance();
    test_normal_queue_read_performance();
    // test_lock_free_queue_write_performance();
    // test_normal_queue_write_performance();
    // test_lock_free_queue_comprehensive_performance();
    return 0;
}
