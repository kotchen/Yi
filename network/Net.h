#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstring>
#include <memory>
#include "socket/Socket.h"
#include "../thread/LockFreeThreadPool.h"
#include "../util/Singleton.h"
namespace yi
{
    class Net
    {
    private:
        /* data */
        static const size_t MAXEVENTS = 100;
        static const size_t TASK_QUEUE_CAPACITY = 1024;
        std::shared_ptr<yi::Socket> _listen_sock;
        std::shared_ptr<yi::PackageParser> _parser;
        struct sockaddr_in _server_addr;
        int _epollfd;
        struct epoll_event events[MAXEVENTS];

    public:
        Net(std::shared_ptr<yi::Socket> listen_sock, std::shared_ptr<yi::PackageParser> parser) 
            : _listen_sock(listen_sock), 
              _parser(parser)
        {
            
            memset(&_server_addr, 0, sizeof(_server_addr));
            _epollfd = epoll_create(1);

            int32_t cpu_nums = std::thread::hardware_concurrency();
            yi::Singleton<yi::LockFreeThreadPool>::New(cpu_nums, TASK_QUEUE_CAPACITY); 
        }
        ~Net()
        {
            close(_epollfd);
            yi::Singleton<yi::LockFreeThreadPool>::Delete();
        }

        bool Connect(const std::string &ip, int port)
        {
            _server_addr.sin_family = _listen_sock->GetDomain();
            _server_addr.sin_port = htons(port);
            _server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

            int res = connect(_listen_sock->GetSockFd(), (struct sockaddr *)&_server_addr, sizeof(_server_addr));
            if (res != 0)
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

        bool Bind(const std::string &ip, int port)
        {
            _server_addr.sin_family = _listen_sock->GetDomain();
            _server_addr.sin_port = htons(port);
            _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

            int res = bind(_listen_sock->GetSockFd(), (struct sockaddr *)&_server_addr, sizeof(_server_addr));
            if (res != 0)
            {
                printf("bind error\n");
                return false;
            }
            return true;
        }

        bool Listen()
        {
            int res = listen(_listen_sock->GetSockFd(), 5);
            if (res != 0)
            {
                printf("listen error!\n");
                return false;
            }
            else
                return true;
        }

        bool Accept()
        {
            struct epoll_event ev;
            ev.data.fd = _listen_sock->GetSockFd();
            ev.events = EPOLLIN;
            epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listen_sock->GetSockFd(), &ev);
            while (true)
            {
                // 等待监视的socket有事件发生。
                int infds = epoll_wait(_epollfd, events, MAXEVENTS, -1);
                // printf("epoll_wait infds=%d\n",infds);

                // 返回失败。
                if (infds < 0)
                {
                    if (errno != EINTR)
                    {
                        printf("epoll_wait() failed.\n");
                        perror("epoll_wait()");
                        break;
                    }
                    continue;
               }

                // 超时。
                if (infds == 0)
                {
                    printf("epoll_wait() timeout.\n");
                    continue;
                }

                // 遍历有事件发生的结构数组。
                for (int ii = 0; ii < infds; ii++)
                {
                    if ((events[ii].data.fd == _listen_sock->GetSockFd()) && (events[ii].events & EPOLLIN))
                    {
                        // 如果发生事件的是listensock，表示有新的客户端连上来。
                        struct sockaddr_in client;
                        socklen_t len = sizeof(client);
                        int clientsock = accept(_listen_sock->GetSockFd(), (struct sockaddr *)&client, &len);
                        if (clientsock < 0)
                        {
                            printf("accept() failed.\n");
                            continue;
                        }

                        // 把新的客户端添加到epoll中。
                        memset(&ev, 0, sizeof(struct epoll_event));
                        ev.data.fd = clientsock;
                        ev.events = EPOLLIN | EPOLLOUT;
                        epoll_ctl(_epollfd, EPOLL_CTL_ADD, clientsock, &ev);
                        // connect callback 
                        // send function info to client
                        _connect_callback();
                        printf("client(socket=%d) connected ok.\n", clientsock);

                        continue;
                    }
                    else if (events[ii].events & EPOLLIN)
                    {
                        // 客户端有数据过来或客户端的socket连接被断开。
                        char buffer[8*(1<<10)];
                        memset(buffer, 0, sizeof(buffer));

                        // 读取客户端的数据。
                        ssize_t isize = read(events[ii].data.fd, buffer, sizeof(buffer));

                        // 发生了错误或socket被对方关闭。
                        if (isize <= 0)
                        {
                            printf("client(eventfd=%d) disconnected.\n", events[ii].data.fd);

                            // 把已断开的客户端从epoll中删除。
                            memset(&ev, 0, sizeof(struct epoll_event));
                            ev.events = EPOLLIN;
                            ev.data.fd = events[ii].data.fd;
                            epoll_ctl(_epollfd, EPOLL_CTL_DEL, events[ii].data.fd, &ev);
                            close(events[ii].data.fd);
                            continue;
                        }

                        // _parser->

                        // printf("recv(eventfd=%d,size=%d):%s\n", events[ii].data.fd, isize, buffer);
                        // 当前fd 注册epollout
                    }
                }
            }
        }
    };


} // namespace yi
