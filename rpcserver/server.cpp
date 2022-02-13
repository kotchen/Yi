#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <set>
#include "RpcServer.h"

//
// 这里的函数不能使用模板来设计，因为在编译期的时候是不知道函数的存在的
// 没有实例化函数
std::vector<std::any> add(std::vector<std::any> params)
{
    std::vector<std::any> ret_v;
    int ret = 0;
    for (int i = 0; i < params.size(); i++)
    {
        ret += std::any_cast<int>(params[i]);
    }
    ret_v.push_back(ret);
    return ret_v;
}

std::map<std::string, std::string> func_map;
std::set<std::string> func_set;

int main(int argc, char const *argv[])
{
    // 创建服务对象
    yi::RpcServer server;

    // 登记服务函数
    server.register_function("add", add);
    // 启动服务，给出port and ip
    server.start(2000, "127.0.0.1");
    return 0;
}
