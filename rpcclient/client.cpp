#include "RpcClient.h"
#include <vector>
#include <iostream>
#include <any>
int main(int argc, char const *argv[])
{
    yi::RpcClient client(AF_INET, SOCK_STREAM);
    client.Connect("127.0.0.1", 2000);
    client.CallFunction("add", 1, 1);    
    // std::cout << std::any_cast<int> (ret.begin()) << '\n';
    return 0;
}
