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
namespace yi
{

    class RpcServer
    {

    private:
        std::unordered_map<std::string, std::function<void()>> _function_call_map;
        std::shared_ptr<yi::Net> _net;
        std::shared_ptr<yi::Socket> _listen_sock;
        std::shared_ptr<yi::PackageParser> _parser;

        void _DoConnect()
        {

        }

    public:
        RpcServer(size_t threads)
        {
            _listen_sock = std::make_shared<yi::Socket>(AF_INET, SOCK_STREAM);
            _parser = std::make_shared<yi::PackageParser>();
            _net = std::make_shared<yi::Net>(_listen_sock, _parser);

            yi::Singleton<yi::LockFreeThreadPool>::New(threads);
            
        }

        RpcServer(int domain, int type)
        {
            _listen_sock = std::make_shared<yi::Socket>(domain, type);
            _net = std::make_shared<yi::Net>(_listen_sock);
        }

        ~RpcServer()
        {
            yi::Singleton<yi::LockFreeThreadPool>::Delete();
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