#include <arpa/inet.h>
#include <cstring>
namespace yi
{
    class Net
    {
    private:
        /* data */
        struct sockaddr_in _client_addr; 
        struct sockaddr_in _server_addr; 
    public:
        Net(/* args */)
        {
            memset(&_client_addr, 0, sizeof(_client_addr));
            memset(&_server_addr, 0, sizeof(_server_addr));
        }
        ~Net()
        {
        }

        bool Connect(const std::string &ip, int port, const yi::Socket& sock)
        {
            _server_addr.sin_family = sock.GetDomain();
            _server_addr.sin_port  = htons(port);
            _server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

            int res = connect(sock.GetSockFd(), (struct sockaddr*)&_server_addr, sizeof(_server_addr));
            if (res!=0)
            {
                printf("connection error\n");
                return false;
            }            
            else
            {
                printf("connection!\n");
                return true;
            } 
        }
    };

} // namespace yi
