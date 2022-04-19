#include "Request.pb.h"
#include <map>

yi::FunctionRet::AddIntRet add_int(const yi::FunctionCall::AddIntParams& add_int_params)
{
    yi::FunctionRet::AddIntRet ret;
    ret.set_ret(add_int_params.left()+add_int_params.right());
    return ret;
}

yi::FunctionRet::AddDoubleRet add_double(const yi::FunctionCall::AddDoubleParams& add_double_params)
{
    yi::FunctionRet::AddDoubleRet ret;
    ret.set_ret(add_double_params.left()+add_double_params.right());
    return ret;
}

yi::FunctionRet::AddStringRet add_string(const yi::FunctionCall::AddStringParams& add_string_params)
{
    yi::FunctionRet::AddStringRet ret;
    ret.set_ret(add_string_params.left()+add_string_params.right());
    return ret;
}

yi::FunctionRet::PlayerMoveRet player_move(const yi::FunctionCall::PlayerMoveParams& player_move_params)
{
    yi::FunctionRet::PlayerMoveRet ret;
    
    return ret;
}



template<typename T>
constexpr yi::Request MakeRequest(yi::Request::RequestType request_type, T request)
{
    yi::Requset req;
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

template<typename T, typename... Args>
T MakeRequestBody(yi::Request::RequestType request_type)
{

}

void status_mechine(const std::string& package)
{
    yi::Request req;
    req.ParseFromString(package);
    switch (req.call_type())
    {
        case yi::Request::ConnectionReq:
            break;
        case yi::Request::ConnectionRet:
            // for (auto& item : ret.connection_ret().func_info())
            // {
            //     // std::cout << item.func_name() << " " << item.params_type() << " " << item.return_type() << std::endl;
            //     client_function_list[item.func_name()] = {item.params_type(), item.return_type()};
            // }
            break;
        case yi::Request::FunctionCall:
            break;
        case yi::Request::FunctionRet:
            break;
        default:
            break;
    }
}






int main(int argc, char const *argv[])
{

    std::unordered_map<std::string, std::pair<std::string, std::string>> params;
    // 服务器 设置好函数的参数和返回值的信息
    params["add_int"] = {"ii", "i"};
    params["add_double"] = {"dd", "d"};
    params["add_string"] = {"ss", "s"};
    params["player_move"] = {"ddddd", "dddd"};
    
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
        yi::FunctionCall::PlayerMoveParams player_move_params;
        player_move_params.set_x(1.0);
        player_move_params.set_y(2.0);
        player_move_params.set_acceleration(3.0);
        player_move_params.set_speed(4.0);
        player_move_params.set_angle(5.0);
        player_move_params.set_aspect(6.0);

        player_move.mutable_player_move_params()->CopyFrom(player_move_params);

        auto PlayerMoveReq = MakeRequest(yi::Request::FunctionCall, player_move);



        std::string out;
        PlayerMoveReq.SerializeToString(&out);

        // 服务器接收客户端发来的二进制信息，然后解包
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

    }

    return 0;
}
