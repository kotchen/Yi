#include "Net.h"
#include <iostream>
#include "serialization/Parser.h"

std::pair<ssize_t, std::shared_ptr<yi::Buffer<uint8_t>>> yi::Net::_DoRead(int fd)
{
    // 读取客户端的数据。
    // 发生了错误或socket被对方关闭。
    auto read_buffer = this->_connection_buffer_pool[fd];
    if (!read_buffer)
        return {0, nullptr};
    auto isize = read_buffer->ReadFromSocket();
    // printf("read: %d\n", isize);
    return {isize, read_buffer};
}

void yi::Net::_DoDisconnect(int fd, struct epoll_event &ev, std::function<void(int, std::shared_ptr<yi::Buffer<uint8_t>>)> write_func)
{
    printf("client(eventfd=%d) disconnected.\n", fd);
    // 把已断开的客户端从epoll中删除。
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
    auto buffer = this->_connection_buffer_pool[fd];
    if (buffer->IsEmpty())
        this->_connection_buffer_pool.erase(fd);
    else
    {
        write_func(fd, buffer);
    }

    this->_connection_buffer_pool.erase(fd);
    close(fd);
    return;
}

void yi::Net::_DoWriteFunctionCall(int fd, std::shared_ptr<yi::Buffer<uint8_t>> ring_buffer)
{
    char buffer[8 * (1 << 10)] = {0};
    std::unique_lock<std::mutex> lock(ring_buffer->GetMutex());
    while (!ring_buffer->IsEmpty())
    {
        uint32_t size = ring_buffer->Readn((uint8_t *)buffer, 2);
        // std::cout << "read size: " << size << "from read_buffer" << std::endl;
        std::string msg_header_str(buffer, size);
        yi::MsgHeader msg_header;
        msg_header.ParseFromString(msg_header_str);
        auto msg_size = msg_header.msg_size();
        // std::cout << "msg_size: " << msg_size << std::endl;
        if (ring_buffer->Remain() < msg_size)
        {
            ring_buffer->Retrive(size);
            return;
        }
        ring_buffer->Readn((uint8_t *)buffer, msg_size);
        std::string msg_body_str(buffer, msg_size);
        auto request = yi::Parse(msg_body_str);
        auto function_call = request.function_call();
        auto function_name = function_call.function_name();
        auto task = std::bind((*_function_call_map)[function_name], function_call);
        auto ret = task();
        // printf("生成了ret\n");
        yi::Request req;
        req.set_call_type(Request_RequestType_FunctionRet);
        req.mutable_function_ret()->CopyFrom(ret);
        // printf("准备发送ret\n");
        this->_Write(fd, req);
    }
}
void yi::Net::_DoWriteFunctionRet(int fd, std::shared_ptr<yi::Buffer<uint8_t>> ring_buffer)
{
    char buffer[8 * (1 << 10)] = {0};
    std::unique_lock<std::mutex> lock(ring_buffer->GetMutex());
    while (!ring_buffer->IsEmpty())
    {
        uint32_t size =ring_buffer->Readn((uint8_t *)buffer, 2);
        // std::cout << "read size: " << size << "from read_buffer" << std::endl;
        std::string msg_header_str(buffer, size);
        yi::MsgHeader msg_header;
        msg_header.ParseFromString(msg_header_str);
        auto msg_size = msg_header.msg_size();
        // std::cout << "msg_size: " << msg_size << std::endl;
        if (ring_buffer->Remain() < msg_size)
        {
            ring_buffer->Retrive(size);
            return;
        }
        ring_buffer->Readn((uint8_t *)buffer, msg_size);
        std::string msg_body_str(buffer, msg_size);
        auto request = yi::Parse(msg_body_str);

        auto function_ret = request.function_ret();
        auto function_name = function_ret.function_name();
        auto task = std::bind((*_function_call_back_map)[function_name], function_ret);
        task();
    }
}

void yi::Net::_ServerRead(int fd, struct epoll_event &ev)
{
    // 客户端有数据过来或客户端的socket连接被断开。
    // 这里需要申请内存，大概是8K的内存，这个内存是用来接收客户端的数据的。
    // 为了效率以及稳定性，需要实现内存池
    auto [isize, read_buffer] = this->_DoRead(fd);
    if (!read_buffer)
        return ;
    if (isize <= 0)
    {
        auto last_function = std::bind(&yi::Net::_DoWriteFunctionCall, this, std::placeholders::_1, std::placeholders::_2);
        this->_DoDisconnect(fd, ev, last_function);
        return;
    }
    // 这里需要while(ture)循环，直到获取了sizeof(yi::Request)大小的数据。
    if (isize < 2)
        return;
    this->_DoWriteFunctionCall(fd, read_buffer);
}

void yi::Net::_ClientRead(int fd, struct epoll_event &ev)
{
    // 客户端有数据过来或客户端的socket连接被断开。
    // 这里需要申请内存，大概是8K的内存，这个内存是用来接收客户端的数据的。
    // 为了效率以及稳定性，需要实现内存池
    auto [isize, read_buffer] = this->_DoRead(fd);
    if (isize <= 0)
    {
        auto last_function = std::bind(&yi::Net::_DoWriteFunctionRet, this, std::placeholders::_1, std::placeholders::_2);
        this->_DoDisconnect(fd, ev, last_function);
        return;
    }
    // 读取客户端的数据。
    // std::cout << "client read isize: " << isize << std::endl;
    // 发生了错误或socket被对方关闭。
    if (isize < 2)
        return;
    this->_DoWriteFunctionRet(fd, read_buffer);
}
ssize_t yi::Net::_Write(int fd, const yi::Request &req)
{
    // 发送数据
    std::string msg_body = yi::Serialize(req);
    yi::MsgHeader msg_header;
    msg_header.set_msg_size(msg_body.size());
    std::string msg_header_str = msg_header.SerializeAsString();
    std::string package = msg_header_str + msg_body;
    auto size = write(fd, package.c_str(), package.size());
    // std::cout << "header size:" << msg_header_str.size() << "body size:" << msg_body.size() << "total size: " << size << std::endl;
    return size;
}

yi::Net::Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>> *function_call_map)
    : _listen_sock(listen_sock),
      _function_call_map(nullptr),
      _function_call_back_map(function_call_map),
      _task_pool(threads)
{
    memset(&_server_addr, 0, sizeof(_server_addr));
    _epollfd = epoll_create(1);
}
yi::Net::Net(size_t threads, std::shared_ptr<yi::Socket> listen_sock, std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall &)>> *function_call_map)
    : _listen_sock(listen_sock),
      _function_call_map(function_call_map),
      _function_call_back_map(nullptr),
      _task_pool(threads)
{

    memset(&_server_addr, 0, sizeof(_server_addr));
    _epollfd = epoll_create(1);
}

yi::Net::~Net()
{
    close(_epollfd);
}

bool yi::Net::Connect(const std::string &ip, int port)
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
        std::shared_ptr<yi::Buffer<uint8_t>> buffer = std::make_shared<yi::Buffer<uint8_t>>(_listen_sock->GetSockFd(), 8 * 1024);
        this->_connection_buffer_pool[_listen_sock->GetSockFd()] = buffer;
        printf("connection!\n");
        return true;
    }
}

void yi::Net::Start()
{
    struct epoll_event ev;
    ev.data.fd = _listen_sock->GetSockFd();
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listen_sock->GetSockFd(), &ev);

    while (true)
    {
        // 等待监视的socket有事件发生。
        int infds = epoll_wait(_epollfd, events, MAXEVENTS, -1);

        // 返回失败。
        if (infds < 0)
        {
            if (errno != EINTR)
            {
                printf("epoll_wait() failed.\n");
                perror("epoll_wait()");
                printf("%s\n", strerror(errno));
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
            if (events[ii].events == EPOLLIN)
            {
                int fd = events[ii].data.fd;
                _task_pool.enqueue(&yi::Net::_ClientRead, this, fd, ev);
            }
        }
    }
}

void yi::Net::Send(int fd, const yi::Request &req)
{
    _Write(fd, req);
}

bool yi::Net::Bind(const std::string &ip, int port)
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

bool yi::Net::Listen()
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

void yi::Net::Accept()
{
    struct epoll_event ev;
    ev.data.fd = _listen_sock->GetSockFd();
    ev.events = EPOLLIN;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listen_sock->GetSockFd(), &ev);
    while (true)
    {
        // 等待监视的socket有事件发生。
        int infds = epoll_wait(_epollfd, events, MAXEVENTS, -1);
        printf("epoll_wait infds=%d\n", infds);

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
            printf("ii=%d\n", ii);
            printf("events[ii].data.fd=%d\n", events[ii].data.fd);
            printf("events[ii].events=%x\n", events[ii].events);
            if ((events[ii].data.fd == _listen_sock->GetSockFd()) && (events[ii].events & EPOLLIN))
            {
                // 如果发生事件的是listensock，表示有新的客户端连上来。
                // 主线程只负责接受新的客户端连接，不负责处理客户端的数据。
                // 因为如果主线程也负责IO的话，那么就会导致主线程阻塞，导致无法接受新的客户端连接。
                printf("创建新的连接\n");
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
                // ev.events = EPOLLIN | EPOLLOUT;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(_epollfd, EPOLL_CTL_ADD, clientsock, &ev);
                {
                    std::lock_guard<std::mutex> lk(_mutex);
                    if (this->_connection_buffer_pool.find(clientsock) == _connection_buffer_pool.end())
                    {
                        std::shared_ptr<yi::Buffer<uint8_t>> buffer = std::make_shared<yi::Buffer<uint8_t>>(clientsock, 8 * 1024);
                        this->_connection_buffer_pool[clientsock] = buffer;
                    }
                }
                printf("client(socket=%d) connected ok.\n", clientsock);
            }
            else if (events[ii].events & EPOLLIN)
            {
                printf("启动server read\n");
                int fd = events[ii].data.fd;
                _task_pool.enqueue(&yi::Net::_ServerRead, this, fd, ev);
            }
        }
    }
}

thread::ThreadPool &yi::Net::GetTaskPool()
{
    return _task_pool;
}