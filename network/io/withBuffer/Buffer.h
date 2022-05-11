#ifndef __YI_BUFFER__
#define __YI_BUFFER__

#include <unistd.h>
#include "util/RingBuffer.h"
#include <mutex>
namespace yi
{
template<typename T>
class Buffer
{
private:
    int _io_fd;
    RingBuffer<T> _ring_buffer; 
    std::mutex _mutex;
public:
    Buffer(int fd, uint32_t capacity)
        : _io_fd(fd), _ring_buffer(capacity)
    {
        
    }
    Buffer(Buffer&& other)
    {
        _io_fd = other._io_fd;
        _ring_buffer = other._ring_buffer;
        other._io_fd = -1;
        other._ring_buffer = nullptr;
    }
    ~Buffer() {}

    uint32_t ReadFromSocket()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        uint8_t buf[1024];
        uint32_t read_size =  read(_io_fd, buf, sizeof(buf));
        return _ring_buffer.Writen(buf, read_size);
    }

    // uint32_t WriteToSocket()
    // {

    // }

    uint32_t Readn(T* buf, uint32_t n)
    {
        return _ring_buffer.Readn(buf, n);
    }

    uint32_t Writen(T* buf, uint32_t n)
    {
        return _ring_buffer.Writen(buf, n);
    }

    bool IsEmpty()
    {
        return _ring_buffer.IsEmpty();
    }

    uint32_t Remain()
    {
        return _ring_buffer.Remain();
    }

    void Retrive(uint32_t n)
    {
        _ring_buffer.Retrive(n);
    }

    std::mutex& GetMutex()
    {
        return _mutex;
    }
};

};

#endif