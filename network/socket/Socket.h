#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include "../network/io/noBuffer/network_io.h"
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

        bool SendPackage(const char* package)
        {
            network_io_writen(_sockfd, package, sizeof(package));
        }

    };
    
     
} // namespace yi
