#include <any>
#include "../util/yi_type.h"
#include "../serialization/PackageParser.h"
#include "noBuffer/network_io.h"
namespace yi
{
    class RpcClient
    {
    private:
        /* data */
        yi::PackageParser _package_parser;
    public:
        RpcClient(/* args */);
        ~RpcClient();
        bool connect(int port, std::string ip)
        {
        }

        template<typename... Args>
        FunctionRet CallFunction(const std::string& function_name, Args&&... args)
        {
            // 先解包参数
            _package_parser.ParseParams(function_name, std::forward<Args>(args)...);
            // 序列化 function name and parameters
            _package_parser.serialize();
            // send package
            char usrbuf[8*(1<<10)];
            network_io_writen()
            // wait return 
        }
    };

    RpcClient::RpcClient(/* args */)
    {
    }

    RpcClient::~RpcClient()
    {
    }

} // namespace yi
