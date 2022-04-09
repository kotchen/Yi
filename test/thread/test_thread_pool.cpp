#include "ThreadPool.h"
#include "LockFreeThreadPool.h"
#include <vector>

void func()
{
    std::vector<int> v{5,4,3,2,1};
    std::sort(v.begin(), v.end());
}



int main(int argc, char const *argv[])
{
    yi::LockFreeThreadPool LockFreePool(4);
    thread::ThreadPool threadPool(4); 

    for (int i = 0; i < 10000; i++)
    {
        LockFreePool.enqueue(func);
    }
    for (int i = 0; i < 10000; i++)
    {
        threadPool.enqueue(func);
    }
}
