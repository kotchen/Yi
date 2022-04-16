#ifndef __YI_PACKGE_PARSER__
#define __YI_PACKGE_PARSER__
#include "FunctionCall.pb.h"

class YiPackageParser
{
private:
    /* data */
    yi::FunctionCall _function_call;

    template<typename... Args>
    void _serialize(Args... parameters)
    {

    }
public:
    YiPackageParser(/* args */);
    ~YiPackageParser();

    template<Args... args>
    auto DeSerialize(const std::string& )

    template<typename... args>
    void Serialize(const std::string& function_name, args&&... parameters)
    {
        _function_call.set_function_name(function_name);

    }


};

YiPackageParser::YiPackageParser(/* args */)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

YiPackageParser::~YiPackageParser()
{
}



#endif