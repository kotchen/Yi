#ifndef __YI_THREAD__
#define __YI_THREAD__

#include <memory>
#include <thread>
#include <atomic>
namespace yi
{
class Thread
{
protected:
    std::atomic<bool>               _stop;
    std::unique_ptr<std::thread>    _thread;

public:
    Thread();
    virtual ~Thread();
    Thread (const Thread&)              = delete;
    Thread& operator= (const Thread&)   = delete; 
    virtual void Start();
    virtual void Stop();
    virtual void Join();
    virtual void Run()                  = 0;

    virtual bool IsStop();
};

};

#endif