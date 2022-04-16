#include "ThreadPool.h"
#include "LockFreeThreadPool.h"
#include <vector>
#include <iostream>
using namespace std;
void func()
{
    std::vector<int> v{5, 4, 3, 2, 1};
    std::sort(v.begin(), v.end());
}
void test(std::promise<int> &para)
{
    para.set_value(10);
    para.set_value(20);
    return;
}

std::future<int> funtion(int a, int b)
{
    
}

int main(int argc, char const *argv[])
{
    std::promise<int> pro;
    std::future<int> T = pro.get_future();
    std::thread fun(test, std::ref(pro));
    fun.join();
    cout << T.get() << endl;
    return 0;
}
