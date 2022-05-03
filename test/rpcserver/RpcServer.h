#ifndef __RPC_SERVER__
#define __RPC_SERVER__
#include <functional>
#include <unordered_map>
#include <memory>
#include <any>
#include <map>
#include "../util/yi_type.h"
#include "../network/Net.h"
#include "../network/socket/Socket.h"
#include "../serialization/PackageParser.h"
#include "../util/Singleton.h"
#include "../thread/LockFreeThreadPool.h"
#include "Request.pb.h"
namespace yi
{

    class RpcServer
    {

    private:
        std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall&)>> _function_call_map;
        std::shared_ptr<yi::Net> _net;
        std::shared_ptr<yi::Socket> _listen_sock;

        void _DoConnect()
        {

        }

    public:
        RpcServer(size_t threads)
        {
            _listen_sock = std::make_shared<yi::Socket>(AF_INET, SOCK_STREAM);
            _net = std::make_shared<yi::Net>(_listen_sock, _function_call_map);

        }

        // RpcServer(int domain, int type)
        // {
        //     _listen_sock = std::make_shared<yi::Socket>(domain, type);
        //     _net = std::make_shared<yi::Net>(_listen_sock);
        // }

        ~RpcServer()
        {
        }

        template<typename T>
        bool register_function(const std::string& func_name, T& called_function)
        {
            auto ite = _function_call_map.find(func_name);
            if (ite != _function_call_map.end())
                return false;
            _function_call_map[func_name] = called_function;
            return true;
        }

        void start(const std::string &ip, int port)
        {
            _net->Bind(ip, port);
            _net->Listen();
            _net->Accept();
        }
    };

};
#endif