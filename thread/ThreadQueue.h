#ifndef __THREAD_QUEUE__
#define __THREAD_QUEUE__

#include <queue>
#include "Thread.h"
class ThreadQueue
{
private:
    std::queue<yi::Thread> _queue;
public:
    yi::Thread& Pop()
    {
        
    }
    void Push(const yi::Thread& thread)
    {
        _queue.push(thread);
    }
    size_t Size()
    {
        return _queue.size();
    }
    bool Empty()
    {
        return _queue.empty();
    }

};

#endif