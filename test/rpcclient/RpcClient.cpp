#include "RpcClient.h"

yi::RpcClient::RpcClient(size_t threads)
{
    _listen_sock = std::make_shared<yi::Socket>(AF_INET, SOCK_STREAM);
    _net = std::make_shared<yi::Net>(threads, _listen_sock, _function_call_back_map);
}
yi::RpcClient::~RpcClient()
{
}
// 下面的函数是交给线程
bool yi::RpcClient::Connect(const std::string &ip, int port)
{
    _net->Connect(ip, port);
}

void yi::RpcClient::SetCallBack(const std::string &func_name, std::function<void(const yi::FunctionRet &)> func)
{
    _function_call_back_map[func_name] = func;
}

void yi::RpcClient::CallFunction(const std::string &func_name, const yi::FunctionCall &func_call)
{
    auto req = yi::MakeRequest(yi::Request::FunctionCall, func_call);
    auto package = yi::Serialize(req);
}

void yi::RpcClient::start()
{
    
}