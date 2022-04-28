#ifndef __YI_PARSER__
#define __YI_PARSER__
#include "Request.pb.h"
#include <string>


#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)


#define  COUNT_ARG(...)  ARG_N_HELPER(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define  ARG_N_HELPER(...)  ARG_N(__VA_ARGS__)
#define  ARG_N(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,N,...)  N

/**
 * @brief 配合COUNT_ARG使用的宏，最大可接受的参数为15, 有参数上限的更新可以自行添加，拓展参数个数
 * 
 */

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
/**
 * @brief 设置一个function call的参数
 * 包括函数名和函数的参数   
 * 
 */

#define YI_SET_FUNCTIONCALL_PARAMS(func_name, func_call, ...) \
    yi::FunctionCall::func_name##Params func_call; \
    YI_PARAMS_FILL_IMPL(func_call, __VA_ARGS__)

/**
 * @brief  创建一个function call,并且设置function call的参数，使用宏自动生成代码，而不需要手动进行set
 *         让整个过程更加清爽，更加简洁
 * 
 * @param func_call_name 就是等会传入CallFunction的参数的名字
 * @param func_name 函数的名字，驼峰命名法，开头大写
 * @param params_type 函数的名字小写，用下划线分割，最后加上paramss后缀
 * 
 * @example YI_CREATE_FUNCTIONCALL(player_move_call, PlayerMove, player_move_params, x, 1.0, y, 2.0, .....)
 * 写好前面3个参数之后，后面就是参数的设置，格式为参数名与参数的值成对出现
 * 
 */
#define YI_CREATE_FUNCTIONCALL(func_call_name, func_name, params_type, ...) \
    yi::FunctionCall func_call_name; \
    func_call_name.set_function_name(#func_name); \
    YI_SET_FUNCTIONCALL_PARAMS(func_name, params, __VA_ARGS__) \
    func_call_name.mutable_##params_type()->CopyFrom(params);



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
    /**
     * @brief 序列化一个yi::Rquest 对象
     * 
     * @param req  
     * @return std::string 返回序列化后的字符串
     */

    std::string Serialize(const yi::Request &req)
    {
        std::string serialized;
        req.SerializeToString(&serialized);
        return serialized;
    }

    /**
     * @brief 解析一个std::string对象，并返回一个yi::Request对象
     * 
     * @param str 
     * @return yi::Request 返回一个yi::Request对象
     */

    yi::Request Parse(const std::string &str)
    {
        yi::Request parsed;
        parsed.ParseFromString(str);
        return parsed;
    }
}

#endif