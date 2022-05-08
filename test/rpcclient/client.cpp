#include <vector>
#include <iostream>
#include <chrono>
#include "serialization/Parser.h"
#include "RpcClient.h"

#include "serialization/Request.pb.h"
std::mutex _mutex;
void player_move_call_back(const yi::FunctionRet &ret)
{
    yi::FunctionRet::PlayerMoveRet player_move_ret = ret.player_move_ret_params();
    std::cout << "player_move_call_back: " << player_move_ret.x() << " " << player_move_ret.y() << " " << player_move_ret.speed() << " " << player_move_ret.aspect() << std::endl;
}

void add_int_call_back(const yi::FunctionRet &ret)
{
    yi::FunctionRet::AddIntRet add_int_ret = ret.add_int_ret_params();
    std::cout << "add_int_call_back: " << add_int_ret.ret() << std::endl;
}

int main(int argc, char const *argv[])
{
    yi::RpcClient client(4);
    client.SetCallBack("PlayerMove", player_move_call_back);
    client.SetCallBack("AddInt", add_int_call_back);
    YI_CREATE_FUNCTIONCALL(PlayerMoveReq, PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)
    YI_CREATE_FUNCTIONCALL(AddIntReq, AddInt, add_int_params, left, 1, right, 2)

    client.Connect("127.0.0.1", 5005);
    client.Start();
    while (true)
    {
        client.CallFunction("PlayerMove", PlayerMoveReq);
        client.CallFunction("AddInt", AddIntReq);
        // use chrono to sleep 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    return 0;
}