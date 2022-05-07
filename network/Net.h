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

        thread::ThreadPool _task_pool;

        std::mutex _mutex;
        #ifdef __RPC_CLIENT__
        std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>> &_function_call_map;
        #else
        std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>> &_function_call_map;
        #endif

        void _Read(int fd, struct epoll_event &ev);
        ssize_t _Write(int fd, yi::Request &req);

    public:

        #ifdef __RPC_CLIENT__
        Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>> &function_call_map);
        #else
        Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>> &function_call_map);
        #endif
        ~Net();
        // 客户端
        bool Connect(const std::string &ip, int port);
        void Start();

        // 服务器
        bool Bind(const std::string &ip, int port);
        bool Listen();
        void Accept();
        thread::ThreadPool& GetTaskPool();
    };

} // namespace yi
