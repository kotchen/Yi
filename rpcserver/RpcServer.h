#ifndef __RPC_SERVER__
#define __RPC_SERVER__
#include <functional>
#include <unordered_map>
#include <any>
#include "../util/yi_type.h"
#include "../network/Net.h"
#include "../network/socket/Socket.h"
#include "../serialization/PackageParser.h"
namespace yi
{

class RpcServer
{

private:

    std::unordered_map<std::string,  Function> _function_mapping; 
    yi::Net   _net;
    yi::Socket _sock;
    yi::PackageParser _parser;
public:

    RpcServer() : _sock(AF_INET, SOCK_STREAM)
    {

    }

    RpcServer(int domain, int type) : _sock(domain, type)
    {

    }
    bool register_function(std::string func_name, Function called_function);
    void start(int port, std::string ip)
    {

    }
};

};
#endif