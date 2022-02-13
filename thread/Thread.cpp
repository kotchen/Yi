#include "Thread.h"
#include <functional>
namespace yi
{

Thread::Thread() : _stop(true)
{
}

Thread::~Thread()
{
}

bool Thread::IsStop()
{
    return _stop;
}

void Thread::Start()
{
    _stop = false;
    if (!_thread && !IsStop())
    {
        _thread = std::make_unique<std::thread> (std::bind(&Thread::Run, this));
    }
}

void Thread::Stop()
{
    _stop = true;
}

void Thread::Join()
{
    if (_thread && _thread->joinable())
        _thread->join();
}


};
