#ifndef __YI_PARSER__
#define __YI_PARSER__
#include "Request.pb.h"


#define Foo_ 0
#define Foo_1 1
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)


// yi::FunctionCall::PlayerMoveParams player_move_params;
// player_move_params.set_x(1.0);
// player_move_params.set_y(2.0);
// player_move_params.set_acceleration(3.0);
// player_move_params.set_speed(4.0);
// player_move_params.set_angle(5.0);
// player_move_params.set_aspect(6.0);




#define  COUNT_ARG(...)  ARG_N_HELPER(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define  ARG_N_HELPER(...)  ARG_N(__VA_ARGS__)
#define  ARG_N(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,N,...)  N


#define YI_PARAMS_FILL_2(func_call)

#define YI_PARAMS_FILL_3(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_4(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_5(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_6(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_7(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_8(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_9(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_10(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_11(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
#define YI_PARAMS_FILL_12(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)
// #define YI_PARAMS_FILL_3(func_call, property, value, ...) \
//     func_call.set_##property(value); \
//     (YI_PARAMS_FILL_##YI_PARAMS_COUNT(__VA_ARGS__))(func_call, __VA_ARGS__)
// #define YI_PARAMS_FILL_3(func_call, property, value, ...) \
//     func_call.set_##property(value); \
//     (YI_PARAMS_FILL_##YI_PARAMS_COUNT(__VA_ARGS__))(func_call, __VA_ARGS__)
// #define YI_PARAMS_FILL_3(func_call, property, value, ...) \
//     func_call.set_##property(value); \
//     (YI_PARAMS_FILL_##YI_PARAMS_COUNT(__VA_ARGS__))(func_call, __VA_ARGS__)




#define YI_PARAMS_FILL_IMPL(func_call, property, value, ...) \
    func_call.set_##property(value); \
    CAT(YI_PARAMS_FILL_, COUNT_ARG(func_call, ##__VA_ARGS__))(func_call, ##__VA_ARGS__)

// #define GETMACRO(_1, NAME, ...) NAME

// #define YI_PARAMS_FILL(...) \
//     GETMACRO(__VA_ARGS__, YI_PARAMS_FILL_end, YI_PARAMS_FILL_IMPL) (__VA_ARGS__) \
//     YI_PARAMS_FILL(__VA_ARGS__)


#define YI_SET_FUNCTIONCALL(func_name, func_call, ...) \
    yi::FunctionCall::func_name##Params func_call; \
    YI_PARAMS_FILL_IMPL(func_call, __VA_ARGS__)


YI_SET_FUNCTIONCALL(PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)



namespace yi
{

    template <typename T>
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

    std::string Serialize(const yi::Request &req)
    {
        std::string serialized;
        req.SerializeToString(&serialized);
        return serialized;
    }

    yi::Request Parse(const std::string &req)
    {
        yi::Request parsed;
        parsed.ParseFromString(req);
        return parsed;
    }
}

#endif