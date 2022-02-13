#ifndef __RPC_SERVER__
#define __RPC_SERVER__
#include <functional>
#include <unordered_map>
#include <any>
#include "../util/yi_type.h"
namespace yi
{

class RpcServer
{

private:
    std::unordered_map<std::string,  Function> _function_mapping; 
public:
    bool register_function(std::string func_name, Function called_function);
    void start(int port, std::string ip)
    {

    }
};

};
#endif