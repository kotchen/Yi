/*
work thread

design for dealing with 
    connect
    accept
    read
    write
    disconnect

*/
#include "Thread.h"
class WorkThread : public yi::Thread
{

public:
    virtual void Run();
protected:

};