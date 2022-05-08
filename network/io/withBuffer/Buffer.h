#ifndef __NETWORK_IO_WITH_BUFFER
#define __NETWORK_IO_WITH_BUFFER

#define IO_BUFFER_SIZE (8*(1<<10))
#include <any>
#include <unistd.h>
namespace yi
{
class Buffer
{
private:
    int _io_fd;
    size_t _io_cnt;
    char* _io_buffer_cur_ptr;
    char _io_buffer[IO_BUFFER_SIZE];
    ssize_t _read(char* usrbuf, size_t n);
    ssize_t _write(const char* package, size_t n);
public:
    Buffer() {}
    ~Buffer() {}
    void ioReadInit(int fd);
    ssize_t readn(std::any usrbuf, size_t n);
    ssize_t readLine(std::any usrbuf, size_t maxlen);
    ssize_t sendn(std::any package, size_t n);
};

};

#endif