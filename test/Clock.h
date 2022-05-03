#ifndef __YI_CLOKE__
#define __YI_CLOKE__

// use c++ RAII write a class to compute the time that a function takes
// to execute

#include <chrono>
#include <iostream>

namespace yi
{

    class Clock
    {
    public:
        Clock()
        {
            start = std::chrono::high_resolution_clock::now();
        }

        ~Clock()
        {
            end = std::chrono::high_resolution_clock::now();
            std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
        }

    private:
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
    };
}

#endif