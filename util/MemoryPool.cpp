#include "MemoryPool.h"
#include <iostream>
#include <chrono>
#include <random>
#include <set>
struct Entity
{
    std::string name;
    int age;


    Entity(const std::string& name, int age)
    {
        this->name = name;
        this->age = age;
    }
};
// int main(int argc, char const *argv[])
// {
//     yi::MemoryPool pool(1024);

//     auto p = pool.alloc(512);
//     auto p1 = pool.alloc(512);

//     pool.free(p1);
//     p1 = pool.alloc(300);
//     pool.free(p);
//     p = pool.alloc(300);

// }
int main() {

    constexpr uint32_t capacity = 1024 * 1024;
    constexpr uint32_t minimum = 128;

	// ugi::MemoryAllocator<TLSF_kusugawa> allocator;
    yi::MemoryPool allocator(capacity);
    // allocator.initialize(capacity, minimum);
    std::default_random_engine randEngine;
    std::uniform_int_distribution<uint32_t> randRange(96, 1024);

    uint64_t allocateTime = 0;
    uint64_t freeTime = 0;
    uint32_t allocateCount = 0;
    uint32_t allocatedTotal = 0;

    std::set<void*> pointers;
    uint32_t randSize = randRange(randEngine);
    // std::chrono::steady_clock::time_point startTime;
    // std::chrono::steady_clock::time_point endTime;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto ptr = allocator.alloc(randSize);
    auto endTime = std::chrono::high_resolution_clock::now();
    while(ptr) {
        allocatedTotal += randSize;
        auto duration = endTime - startTime;
        allocateTime += duration.count();
        pointers.insert(ptr);
        randSize = randRange(randEngine);
        randSize = (randSize + 15)&~(15);
        startTime = std::chrono::high_resolution_clock::now();
        ptr = allocator.alloc(randSize);
        endTime = std::chrono::high_resolution_clock::now();
    }
    allocateCount = pointers.size();
    //===================================================================
    while(!pointers.empty()) {
        auto size = pointers.size();
        std::uniform_int_distribution<uint32_t> range(0, size-1);
        auto position = range(randEngine);
        auto iter = pointers.begin();
        for(size_t i = 0; i<position; ++i) {
            ++iter;
        }
        startTime = std::chrono::high_resolution_clock::now();
        allocator.free(*iter);
        endTime = std::chrono::high_resolution_clock::now();
        auto duration = endTime - startTime;
        freeTime += duration.count();
        pointers.erase(iter);
    }

    printf("count : %u\n", allocateCount);
    printf("allocate : %f ms\n", (float)allocateTime/allocateCount/1000000.0f);
    printf("free : %f ms\n", (float)freeTime / allocateCount / 1000000.0f);
    printf("utilization ratio: %f", (float)allocatedTotal/capacity);
    
	return 0;
}
