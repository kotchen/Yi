#include "Buffer.h"
#include <errno.h>
#include <algorithm>
#include <cstring>
namespace yi
{

void Buffer::ioReadInit(int fd)
{
    this->_io_fd = fd;
    this->_io_cnt = 0;
    this->_io_buffer_cur_ptr = this->_io_buffer;
    memset(this->_io_buffer, 0, IO_BUFFER_SIZE);
}

ssize_t Buffer::_read(char* usrbuf, size_t n)
{
    size_t cnt = 0;
    while (this->_io_cnt <= 0)
    {
        this->_io_cnt = read(this->_io_fd, this->_io_buffer, sizeof(this->_io_buffer));
        if (this->_io_cnt < 0)
        {
            if (errno != EINTR)
                return -1;
        } 
        else if (this->_io_cnt == 0)
            return 0;
        else
            this->_io_buffer_cur_ptr = this->_io_buffer;
    }
    cnt = std::min(n, this->_io_cnt);
    memcpy(usrbuf, this->_io_buffer, cnt);
    this->_io_buffer_cur_ptr += cnt;
    this->_io_cnt -= cnt;
    return cnt;
}

ssize_t Buffer::_write(const char* package, size_t n)
{
    
}

ssize_t Buffer::readn(std::any usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;
    char* buffer = std::any_cast<char*> (usrbuf);

    while (nleft>0)
    {
        if ((nread = this->_read(buffer, nleft))<0)
            return -1;
        else if (nread == 0)
            break;
        nleft -= nread;
        buffer += nread;
    }
    return (n-nleft);
}

ssize_t Buffer::readLine(std::any usrbuf, size_t maxlen)
{
    int n, rc;
    char c;
    char* buffer = std::any_cast<char*> (usrbuf);
    for (n = 1; n<maxlen; n++)
    {
        if ((rc = _read(&c, 1) == 1))
        {
            *buffer++ = c;
            if (c == '\n')
            {
                n++;
                break;
            }
        }
        else if (rc == 0)
        {
            if (n==1)
                return 0;
            else 
                break;
        }
        else 
            return -1;
    }
    *buffer = 0;
    return n-1;
}

ssize_t Buffer::sendn(std::any package, size_t n)
{

}

};