#ifndef __YI_TYPE_DESIGN__
#define __YI_TYPE_DESIGN__

#include <functional>
#include <vector>
#include <any>
#include <tuple>
// that function we write to call, such as add
namespace yi
{
using Function = std::function<std::vector<std::any>(std::vector<std::any>)>;

// string : function  name
// vector ： parameters list
using FunctionCallInfo = std::tuple<std::string, std::vector<std::any>>;

using ConnectCallBack = std::function<void()>;

using FunctionCall_CallBack = std::function<std::vector<std::any>(std::vector<std::any>)>;

enum CallType
{
    CONNECTION = 0,
    DISCONNECTION = 1,

    FUNCTIONCALLMAP = 2,
    FUNCTIONCALLINFO = 3,

};

}


#endif