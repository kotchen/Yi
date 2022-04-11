#include "LockFreeQueue.h"
#include <thread>

yi::LockFreeQueue<int> q(10000);

void Producer()
{
    for (int i = 0; i<1000; i++)
    {
        while (true)
        {
            if (q.Push(i))
                break;
        }
    }
}


int main(int argc, char const *argv[])
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++)
    {
        threads.push_back(std::thread(Producer));
    }
    
    for (auto &t : threads)
    {
        t.join();
    }
    
    std::vector<int> res;
    int cur = 0;
    while (true)
    {
        auto ret = q.Pop(cur);
        if (ret)
        {
            res.push_back(cur);
        }
        else
        {
            break;
        }
    }

    std::cout<< "res size: " << res.size() << std::endl;

    return 0;

}
