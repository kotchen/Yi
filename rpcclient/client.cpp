#include "RpcClient.h"
#include <vector>
#include <iostream>
#include <any>
int main(int argc, char const *argv[])
{
    yi::RpcClient client;
    client.connect(2000, "127.0.0.1");
    auto ret = client.CallFunction("add", 1, 1);    
    std::cout << std::any_cast<int> (ret.begin()) << '\n';
    return 0;
}
