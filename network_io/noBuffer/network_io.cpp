#include "network_io.h"
#include <any>
#include <errno.h>
namespace yi
{
ssize_t network_io_readn(int fd, std::any usrbuf, size_t n)
{
    char* buffer = std::any_cast<char*> (usrbuf);
    size_t nleft = n;
    ssize_t nread = 0;
    while (nleft>0)
    {
        if ((nread = read(fd, buffer, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if (nread == 0)
            break;
        nleft -= nread;
        buffer += nread;
    }
    return (n-nleft);
}
ssize_t network_io_writen(int fd, std::any usrbuf, size_t n)
{
    char* buffer = std::any_cast<char*> (usrbuf);
    size_t nleft = n;
    ssize_t nwritten = 0;
    while (nleft>0)
    {
        if ((nwritten = write(fd, buffer, nleft)) <= 0)
        {
            if (errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        buffer += nwritten;
    }
    return n; 
}

};