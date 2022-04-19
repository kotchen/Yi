#ifndef __YI_PARSER__
#define __YI_PARSER__
#include "Request.pb.h"


namespace yi
{


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

    std::string Serialize(const yi::Request& req)
    {
        std::string serialized;
        req.SerializeToString(&serialized);
        return serialized;
    }

    yi::Request Parse(const std::string& req)
    {
        yi::Request parsed;
        parsed.ParseFromString(req);
        return parsed;
    }
}



#endif