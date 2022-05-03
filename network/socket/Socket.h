#ifndef __YI_SOCKET__
#define __YI_SOCKET__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
// #include "../network/io/noBuffer/network_io.h"
namespace yi
{
    class Socket
    {
    private:
        /* data */
        int _sockfd;
        int _domain;
        int _type;
    public:
        Socket(int domain, int type) : _domain(domain), _type(type)
        {
            _sockfd = socket(domain, type, 0);
            if (_sockfd<0)
            {
                printf("socket() failed!");
                exit(0);
            }
            // memset(&_addr, 0, sizeof(_addr));
        }
        ~Socket()
        {
            close(_sockfd);
        }

        int GetSockFd() const
        {
            return this->_sockfd;
        }

        int GetDomain() const
        {
            return this->_domain;
        }

        int GetType() const
        {
            return this->_type;
        }

        void SendPackage(const char* package)
        {
            // network_io_writen(_sockfd, package, sizeof(package));
            write(_sockfd, package, strlen(package));
        }

    };
    
     
} // namespace yi
#endif