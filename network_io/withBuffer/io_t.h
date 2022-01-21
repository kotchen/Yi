#ifndef __NETWORK_IO_WITH_BUFFER
#define __NETWORK_IO_WITH_BUFFER

#define IO_BUFFER_SIZE (8*(1<<10))
#include <any>
#include <unistd.h>
namespace yi
{
class io_t
{
private:
    int _io_fd;
    size_t _io_cnt;
    char* _io_buffer_cur_ptr;
    char _io_buffer[IO_BUFFER_SIZE];
    ssize_t _read(char* usrbuf, size_t n);
public:
    io_t() {}
    ~io_t() {}
    void ioReadInit(int fd);
    ssize_t readn(std::any usrbuf, size_t n);
    ssize_t readLine(std::any usrbuf, size_t maxlen);
};

};

#endif