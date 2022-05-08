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
        using FunctionCallMap = typename std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall&)>>;
        using FunctionCallBackMap = typename std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>>;


        static const size_t MAXEVENTS = 100;
        std::shared_ptr<yi::Socket> _listen_sock;
        struct sockaddr_in _server_addr;
        int _epollfd;
        struct epoll_event events[MAXEVENTS];

        thread::ThreadPool _task_pool;

        std::mutex _mutex;

        FunctionCallMap* _function_call_map;
        FunctionCallBackMap* _function_call_back_map;

        void _ServerRead(int fd, struct epoll_event &ev);
        void _ClientRead(int fd, struct epoll_event &ev);
        ssize_t _Write(int fd, const yi::Request &req);

    public:

        Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>>* function_call_map);
        Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>>* function_call_map);
        ~Net();
        // 客户端
        bool Connect(const std::string &ip, int port);
        void Start();
        void Send(int fd, const yi::Request& func_all);

        // 服务器
        bool Bind(const std::string &ip, int port);
        bool Listen();
        void Accept();
        thread::ThreadPool& GetTaskPool();
    };

} // namespace yi
