#include "Request.pb.h"
#include "Parser.h"
#include <map>
#include "../thread/LockFreeThreadPool.h"
#include "../test/Player.h"

yi::FunctionRet add_int(const yi::FunctionCall& func_call)
{
    yi::FunctionRet ret;
    yi::FunctionCall::AddIntParams params = func_call.add_int_params();
    yi::FunctionRet::AddIntRet add_int_ret;
    // 逻辑计算
    add_int_ret.set_ret(params.left() + params.right());
    // 设置好返回值的属性
    ret.set_function_name("add_int");
    ret.mutable_add_int_ret_params()->CopyFrom(add_int_ret);
    return ret;
}

yi::FunctionRet add_double(const yi::FunctionCall& add_double_params)
{
}

yi::FunctionRet add_string(const yi::FunctionCall& add_string_params)
{
}

yi::FunctionRet player_move(const yi::FunctionCall& func_call)
{
    yi::FunctionRet ret;
    yi::FunctionCall::PlayerMoveParams params = func_call.player_move_params();
    yi::Player palyer;
    // 逻辑计算
    auto ret_params =  palyer.move(params); 
    // 设置好返回值的属性
    ret.set_function_name("player_move");
    ret.mutable_player_move_ret_params()->CopyFrom(ret_params);
    return ret;
}


// call back
void add_int_call_back(const yi::FunctionRet& ret)
{
    yi::FunctionRet::AddIntRet add_int_ret = ret.add_int_ret_params();
    std::cout << "add_int_call_back: " << add_int_ret.ret() << std::endl;
}

void player_move_call_back(const yi::FunctionRet& ret)
{
    yi::FunctionRet::PlayerMoveRet player_move_ret = ret.player_move_ret_params();
    std::cout << "player_move_call_back: " << player_move_ret.x() << " " << player_move_ret.y() << " " << player_move_ret.speed() << " " << player_move_ret.aspect() << std::endl;
}



template<typename T>
constexpr yi::Request MakeRequest(yi::Request::RequestType request_type, T request)
{
    yi::Request req;
    switch (request_type)
    {
        case yi::Request::ConnectionReq:
            req.set_call_type(yi::Request::ConnectionReq);
            break;
        case yi::Request::ConnectionRet:
            req.set_call_type(yi::Request::ConnectionRet);
            req.mutable_connection_ret()->CopyFrom(request); 
            break;
        case yi::Request::FunctionCall:
            req.set_call_type(yi::Request::FunctionCall);
            req.mutable_function_call_req()->CopyFrom(request);
            break;
        case yi::Request::FunctionRet:
            req.set_call_type(yi::Request::FunctionRet);
            req.mutable_function_call_ret()->CopyFrom(request);
            break;
        default:
            break;
    }
    return req;
}


void status_mechine(const yi::FunctionCall& func_call)
{

}






int main(int argc, char const *argv[])
{

    std::unordered_map<std::string, std::pair<std::string, std::string>> params;
    std::unordered_map<std::string, std::function<yi::FunctionRet(const yi::FunctionCall)>> function_call_map;
    std::unordered_map<std::string, std::function<void(const yi::FunctionRet)>> function_ret_map;
    // 服务器 设置好函数的参数和返回值的信息
    params["add_int"] = {"ii", "i"};
    params["add_double"] = {"dd", "d"};
    params["add_string"] = {"ss", "s"};
    params["player_move"] = {"ddddd", "dddd"};

    function_call_map["add_int"] = add_int;
    function_call_map["add_double"] = add_double;
    function_call_map["add_string"] = add_string;
    function_call_map["player_move"] = player_move;

    function_ret_map["add_int"] = add_int_call_back;
    function_ret_map["player_move"] = player_move_call_back;
    
    yi::Request request;

    request.set_call_type(yi::Request::ConnectionRet);
    yi::ConnectionRet connection_ret;
    for (auto& item : params)
    {
        std::cout << item.first << " " << item.second.first << " " << item.second.second << std::endl;
        auto func_info = connection_ret.add_func_info();
        func_info->set_func_name(item.first);
        func_info->set_params_type(item.second.first);
        func_info->set_return_type(item.second.second);
    }
    std::string out;
    request.mutable_connection_ret()->CopyFrom(connection_ret);
    
    request.SerializeToString(&out);
    // std::cout << out << std::endl;

    // 客户端同步服务器发来的函数

    std::cout << "client: " << std::endl;
    std::unordered_map<std::string, std::pair<std::string, std::string>> client_function_list;

    yi::Request ret;
    ret.ParseFromString(out);
    switch (ret.call_type())
    {
    case yi::Request::ConnectionReq:
        break;
    case yi::Request::ConnectionRet:
        for (auto& item : ret.connection_ret().func_info())
        {
            // std::cout << item.func_name() << " " << item.params_type() << " " << item.return_type() << std::endl;
            client_function_list[item.func_name()] = {item.params_type(), item.return_type()};
        }
        break;
    case yi::Request::FunctionCall:
        break;
    case yi::Request::FunctionRet:
        break;
    default:
        break;
    }

    for (auto& item : client_function_list)
    {
        std::cout << item.first << " " << item.second.first << " " << item.second.second << std::endl;
    }


    {
        // 客户端已经完成了 服务器函数的同步
        // 客户端调用函数
        yi::FunctionCall player_move;
        player_move.set_function_name("player_move");

        YI_SET_FUNCTIONCALL_PARAMS(PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)

        player_move.mutable_player_move_params()->CopyFrom(player_move_params);

        auto PlayerMoveReq = yi::MakeRequest(yi::Request::FunctionCall, player_move);



        std::string out;
        PlayerMoveReq.SerializeToString(&out);

        // 服务器接收客户端发来的二进制信息，然后解包

        yi::LockFreeThreadPool pool(4, 4096);
        yi::Request ret;
        ret.ParseFromString(out);
        switch (ret.call_type())
        {
        case yi::Request::ConnectionReq:
            break;
        case yi::Request::ConnectionRet:
            for (auto& item : ret.connection_ret().func_info())
            {
                // std::cout << item.func_name() << " " << item.params_type() << " " << item.return_type() << std::endl;
                client_function_list[item.func_name()] = {item.params_type(), item.return_type()};
            }
            break;
        case yi::Request::FunctionCall:{
            yi::FunctionCall func_call = ret.function_call();
            // 将函数的信息和参数插入线程池中的任务队列
            auto future = pool.enqueue(function_call_map[func_call.function_name()], func_call);
            break;
        }
        case yi::Request::FunctionRet:{
            function_ret_map[ret.function_ret().function_name()](ret.function_ret());
            break;
        }
        default:
            break;
        }

    }

    return 0;
}
