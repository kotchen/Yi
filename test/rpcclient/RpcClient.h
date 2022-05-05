#include <memory>
#include "serialization/Parser.h"
#include "network/Net.h"
#include "network/socket/Socket.h"
#include "Request.pb.h"
#include "thread/ThreadPool.h"
class yi::Request;
namespace yi
{
    class RpcClient
    {
    private:
        /* data */
        std::shared_ptr<yi::Net> _net;
        std::shared_ptr<yi::Socket> _listen_sock;

        std::unordered_map<std::string, std::function<void(const yi::FunctionRet&)>> _function_call_back_map;

        thread::ThreadPool _thread_pool;

    public:
        RpcClient():_thread_pool(2)
        {
            _listen_sock = std::make_shared<yi::Socket> (AF_INET, SOCK_STREAM);
            _net = std::make_shared<yi::Net> (_listen_sock); 
        }
        RpcClient(int domain, int type) : _thread_pool(2)
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
            _net->Connect(ip, port);
        }

        void CallFunction(const yi::FunctionCall& func_call)
        {
            // send package
            yi::Request req = yi::MakeRequest(yi::Request::FunctionCall, func_call);

            _listen_sock->SendPackage(yi::Serialize(req).c_str());

            // wait return
        }

        void SetCallBack(const std::string& func_name, std::function<void(const yi::FunctionRet&)> func)
        {
            _function_call_back_map[func_name] = func;
        }

        void start()
        {
        } 
    };

} // namespace yi
