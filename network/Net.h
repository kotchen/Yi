#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstring>
#include <memory>
#include "socket/Socket.h"
#include "thread/ThreadPool.h"
#include "util/Singleton.h"
#include "serialization/Parser.h"
namespace yi
{
    class Net
    {
    private:
        /* data */
        static const size_t MAXEVENTS = 100;
        static const size_t TASK_QUEUE_CAPACITY = 1024;
        std::shared_ptr<yi::Socket> _listen_sock;
        struct sockaddr_in _server_addr;
        int _epollfd;
        struct epoll_event events[MAXEVENTS];

        thread::ThreadPool _out_pool;
        thread::ThreadPool _task_pool;

        std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>> &_function_call_map;

        void _EpollOut();

        void _Read(int fd, struct epoll_event &ev);
        void _write(int fd, yi::Request &req);

    public:
        Net(std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>> &function_call_map);
        ~Net();

        bool Connect(const std::string &ip, int port);
        bool Bind(const std::string &ip, int port);
        bool Listen();
        void Accept();
    };

} // namespace yi
