#ifndef __RPC_CLIENT__
#define __RPC_CLIENT__
#include <memory>
#include "serialization/Parser.h"
#include "serialization/Request.pb.h"
#include "network/Net.h"
#include "network/socket/Socket.h"
#include "Request.pb.h"
#include "thread/ThreadPool.h"
class yi::Request;
namespace yi
{
    class RpcClient
    {
    private:
        /* data */
        std::unordered_map<std::string, std::function<void(const yi::FunctionRet &)>> _function_call_back_map;
        std::shared_ptr<yi::Net> _net;
        std::shared_ptr<yi::Socket> _listen_sock;

    public:
        RpcClient(size_t threads);
        
        ~RpcClient();

        // 下面的函数是交给线程
        bool Connect(const std::string &ip, int port);
        

        void SetCallBack(const std::string &func_name, std::function<void(const yi::FunctionRet &)> func);
        void CallFunction(const std::string &func_name, const yi::FunctionCall &func_call);
        void start();
    };

} // namespace yi
#endif