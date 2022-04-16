#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <set>
#include "RpcServer.h"

int add_int(int a, int b)
{
    return a + b;
}

double add_double(double a, double b)
{
    return a + b;
}

std::string add_string(const std::string& a, const std::string& b)
{
    return a + b;
}


int main(int argc, char const *argv[])
{
    // 创建服务对象
    yi::RpcServer server(4);

    // 登记服务函数
    server.register_function("add", [](){
        std::bind(add_int, std::placeholders::_1, std::placeholders::_2);
    });
    // 启动服务，给出port and ip
    server.start("127.0.0.1", 2000);
    return 0;
}
