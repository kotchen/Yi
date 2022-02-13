#ifndef __NETWRODK_IO__
#define __NETWRODK_IO__
#include <unistd.h>
#include <any>
namespace yi
{
ssize_t network_io_readn(int fd, std::any usrbuf, size_t n);
ssize_t network_io_writen(int fd, std::any usrbuf, size_t n);
};


#endif