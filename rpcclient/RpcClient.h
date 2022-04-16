#include <any>
#include <memory>
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
        std::shared_ptr<yi::Net> _net;
        std::shared_ptr<yi::Socket> _listen_sock;

    public:
        RpcClient()
        {
            _listen_sock = std::make_shared<yi::Socket> (AF_INET, SOCK_STREAM);
            _net = std::make_shared<yi::Net> (_listen_sock); 
        }
        RpcClient(int domain, int type)
        {
            _listen_sock = std::make_shared<yi::Socket> (domain, type);
            _net = std::make_shared<yi::Net> (_listen_sock); 
        }
        ~RpcClient()
        {

        }
        // 下面的函数是交给线程
        bool Connect(const std::string& ip, int port)
        {
            struct sockaddr_in addr;
            _net->Connect(ip, port);
        }

        template <typename... Args>
        void CallFunction(const std::string &function_name, Args &&...args)
        {
            // 先解包参数
            _package_parser.ParseParams(function_name, std::forward<Args>(args)...);
            // 序列化 function name and parameters
            auto package = _package_parser.Serialize();
            // send package
            _listen_sock->SendPackage(package);
            // wait return
        }
    };

} // namespace yi
