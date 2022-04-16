#include "RpcClient.h"
#include <vector>
#include <iostream>
#include <any>
int main(int argc, char const *argv[])
{
    yi::RpcClient client;
    client.Connect("127.0.0.1", 2000);
    client.CallFunction("add", 1, 1);    
    //
    // client.add(1, 2);


    // std::cout << std::any_cast<int> (ret.begin()) << '\n';
    return 0;
}
