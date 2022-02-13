#ifndef __DISPATCHER__
#define __DISPATCHER__
#include "Thread.h"
class Dispatcher : public yi::Thread
{
private:
    /* data */
public:
    Dispatcher(/* args */);
    ~Dispatcher();
};

Dispatcher::Dispatcher(/* args */)
{
}

Dispatcher::~Dispatcher()
{
}


#endif