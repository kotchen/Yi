#include <any>
#include "../util/yi_type.h"
#include "../serialization/PackageParser.h"
#include "../network/Net.h"
#include "../network/socket/Socket.h"
namespace yi
{
    class RpcClient
    {
    private:
        /* data */
        yi::PackageParser _package_parser;
        yi::Net _net;
        yi::Socket _sock;

    public:
        RpcClient(int domain, int type) : _sock(domain, type)
        {
             
        }
        ~RpcClient()
        {

        }
        // 下面的函数是交给线程
        bool Connect(const std::string& ip, int port)
        {
            struct sockaddr_in addr;
            _net.Connect(ip, port, _sock);
        }

        template <typename... Args>
        void CallFunction(const std::string &function_name, Args &&...args)
        {
            // 先解包参数
            _package_parser.ParseParams(function_name, std::forward<Args>(args)...);
            // 序列化 function name and parameters
            auto package = _package_parser.Serialize();
            // send package
            _sock.SendPackage(package);
            // wait return
        }
    };

} // namespace yi
