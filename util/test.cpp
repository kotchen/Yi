// #pragma GCC optimize(2)
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <string>
#include <thread>
#include <cassert>
#include <iostream>
#include "LockFreeQueue.h"

// yi::LockFreeQueue<int> q(1024);
struct Entity
{
    std::string name;
    int age;
};
yi::LockFreeQueue<Entity> q(1024);

// void consumer() 
// {
// 	for( size_t i = 0; i<ItemCount/2; ++i) {
// 		size_t val;
// 		while(!lfq.pop(val)) {
// 		}
// 		lfqv.push(val);
// 	}
// }
std::atomic<int> customer_index = {0};
//生产者
void Producer()
{
    for (int i = 0; i<100; i++)
    {
		while (true)
		{
			if (q.Push(Entity()))
				break;
		}
    }
}

//消费者
Entity a;
void Customer()
{
    while (true)
    {
        auto ret = q.Pop(a);
        if (ret)
        {
            // printf("%d\n", a);
            customer_index++;
        }
        if (customer_index== 10000)
            break;
    }    
}

int main() 
{
	// std::thread p1(producer1);
	// std::thread p2(producer2);
	// std::thread c1(consumer);
	// std::thread c2(consumer);
	// //
	// p1.join();
	// p2.join();
	// c1.join();
	// c2.join();
	std::vector<std::thread> producers;
    std::vector<std::thread> customers;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i<100; i++)
        producers.emplace_back(Producer);
    for (int i = 0; i<100; i++)
        customers.emplace_back(Customer);
    for (int i = 0; i<100; i++)
        producers[i].join();
    for (int i = 0; i<100; i++)
        customers[i].join();
    // std::thread p1(Producer);
    // // std::thread p2(Producer);
    // // std::thread p3(Producer);
    // // std::thread p4(Producer);

    // std::thread c1(Customer);
    // std::thread c2(Customer);
    // // std::thread c3(Customer);
    // // std::thread c4(Customer);
    // p1.join();
    // p2.join();
    // // p3.join();
    // // p4.join();
    // c1.join();
    // c2.join();
    // // c3.join();
    // // c4.join();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << '\n';

	// for( size_t i = 0; i<ItemCount; ++i ) {
	// 	size_t val;
	// 	bool rst = lfqv.pop(val);
	// 	assert(rst);
	// 	printf("%zu ", val);
	// }

	return 0;
}