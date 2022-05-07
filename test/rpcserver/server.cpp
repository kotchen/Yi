#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <set>
#include "RpcServer.h"
#include "Request.pb.h"
#include "Player.h"
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

yi::FunctionRet player_move(const yi::FunctionCall &func_call)
{
    yi::FunctionRet ret;
    yi::FunctionCall::PlayerMoveParams params = func_call.player_move_params();
    yi::Player palyer;
    // 逻辑计算
    auto ret_params = palyer.move(params);
    // 设置好返回值的属性
    ret.set_function_name("player_move");
    ret.mutable_player_move_ret_params()->CopyFrom(ret_params);
    return ret;
}



int main(int argc, char const *argv[])
{
    // 创建服务对象
    yi::RpcServer server(4);

    // 登记服务函数
    server.register_function("PlayerMove", player_move);
    // 启动服务，给出port and ip
    server.start("127.0.0.1", 5006);
    return 0;



}
