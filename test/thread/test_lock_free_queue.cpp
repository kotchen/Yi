#include "LockFreeThreadPool.h"
#include "ThreadPool.h"
#include "../clock.h"
#include <vector>
#include <thread>
#include <algorithm>
#include <future>
#include <memory>
#include <mutex>
#include "Singleton.h"
void func()
{
    std::vector<int> v{5,4,3,2,1};
    std::sort(v.begin(), v.end());
}


int add(int a, int b)
{
    return a + b;
}



int main(int argc, char const *argv[])
{

    {
        yi::Clock clock;
        for (int i = 0; i<10000; ++i)
            func();
    }

    {
        std::vector<std::thread> threads;
        yi::Clock clock;
        for (int i = 0; i< 4; ++i)
        {
            threads.emplace_back([](){
                for (int i = 0; i<2500; ++i)
                    func();
            });
        }

        for (auto& t : threads)
            t.join();
    }

    

    // Singleton<thread::ThreadPool>::New(4);
    // {
    //     std::mutex mu;
    //     std::vector<std::future<int>> futures;
    //     yi::Clock clock;
    //     for (int i = 0; i < 4; i++)
    //     {
    //         std::async(std::launch::async, [&mu, &futures](){
    //             for (int i = 0; i<2000; ++i)
    //             {
    //                 std::unique_lock<std::mutex> lock(mu);
    //                 futures.push_back(Singleton<thread::ThreadPool>::Get()->enqueue(add, 1, 1));
    //             }
    //         });
    //     }
    //     for (auto& f : futures)
    //     {
    //         f.wait();
    //     }
    //     // for (auto& f : futures)
    //     // {
    //     //     std::cout << f.get() << std::endl;
    //     // }
    // }
    // Singleton<thread::ThreadPool>::Delete();

    yi::Singleton<yi::LockFreeThreadPool>::New(4, 1024);
    {
        std::vector<std::future<int>> futures;
        yi::Clock clock;
        for (int i = 0; i < 1000; i++)
        {
            futures.push_back(yi::Singleton<yi::LockFreeThreadPool>::Get()->enqueue(add, 1, 1));
        }
        for (auto& f : futures)
        {
            f.wait();
        }
        // for (auto& f : futures)
        // {
        //     std::cout << f.get() << std::endl;
        // }
    }
    yi::Singleton<yi::LockFreeThreadPool>::Delete();


    
}
