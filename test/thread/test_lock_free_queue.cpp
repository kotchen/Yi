#include "LockFreeThreadPool.h"
#include "ThreadPool.h"
#include "../clock.h"
#include <vector>
#include <thread>
#include <algorithm>

void func()
{
    std::vector<int> v{5,4,3,2,1};
    std::sort(v.begin(), v.end());
}



int main(int argc, char const *argv[])
{
    yi::LockFreeThreadPool LockFreePool(4);
    thread::ThreadPool threadPool(4); 

    {
        yi::Clock clock;
        for (int i = 0; i < 10000; i++)
        {
            LockFreePool.enqueue(func);
        }
    }

    {
        yi::Clock clock;
        for (int i = 0; i < 10000; i++)
        {
            threadPool.enqueue(func);
        }
    }
}
