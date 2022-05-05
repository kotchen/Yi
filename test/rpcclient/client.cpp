// #include <vector>
// #include <iostream>
// #include <chrono>
// #include "serialization/Parser.h"
// #include "RpcClient.h"

#include "serialization/Request.pb.h"
void player_move_call_back(const yi::FunctionRet &ret)
{
    yi::FunctionRet::PlayerMoveRet player_move_ret = ret.player_move_ret_params();
    std::cout << "player_move_call_back: " << player_move_ret.x() << " " << player_move_ret.y() << " " << player_move_ret.speed() << " " << player_move_ret.aspect() << std::endl;
}

// void add_int_call_back(const yi::FunctionRet &ret)
// {
//     yi::FunctionRet::AddIntRet add_int_ret = ret.add_int_ret_params();
//     std::cout << "add_int_call_back: " << add_int_ret.ret() << std::endl;
// }

// int main(int argc, char const *argv[])
// {
//     yi::RpcClient client;
//     client.SetCallBack("PlayerMove", player_move_call_back);
//     client.SetCallBack("AddInt", add_int_call_back);
//     YI_CREATE_FUNCTIONCALL(PlayerMoveReq, PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)
//     YI_CREATE_FUNCTIONCALL(AddIntReq, AddInt, add_int_params, left, 1, right, 2)

//     client.Connect("127.0.0.1", 2000);
//     while (true)
//     {
//         client.CallFunction(PlayerMoveReq);
//         // use chrono to sleep 1 second
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         client.CallFunction(AddIntReq);
//     }

//     // 最简单的socket客户端

//     return 0;
// }

/*
 * 程序名：client.cpp，此程序用于演示socket的客户端
 * 作者：C语言技术网(www.freecplus.net) 日期：20190525
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "serialization/Request.pb.h"
#include "serialization/Parser.h"

int main(int argc, char *argv[])
{
    // if (argc != 3)
    // {
    //     printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n");
    //     return -1;
    // }

    const char* ip = "127.0.0.1";
    const char* port = "5005";

    // 第1步：创建客户端的socket。
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }

    // 第2步：向服务器发起连接请求。
    struct hostent *h;
    if ((h = gethostbyname(ip)) == 0) // 指定服务端的ip地址。
    {
        printf("gethostbyname failed.\n");
        close(sockfd);
        return -1;
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port)); // 指定服务端的通信端口。
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) // 向服务端发起连接清求。
    {
        perror("connect");
        close(sockfd);
        return -1;
    }

    // char buffer[1024];
    std::string buffer;//     YI_CREATE_FUNCTIONCALL(PlayerMoveReq, PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)
    char recv_buffer[1024];
    // 第3步：与服务端通信，发送一个报文后等待回复，然后再发下一个报文。
    
    YI_CREATE_FUNCTIONCALL(PlayerMoveReq, PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)
    yi::Request req = yi::MakeRequest(yi::Request::FunctionCall, PlayerMoveReq);

    for (int ii = 0; ii < 3; ii++)
    {
        int iret;
        // memset(buffer, 0, sizeof(buffer));
        buffer = yi::Serialize(req);
        if ((iret = send(sockfd, buffer.c_str(), buffer.size(), 0)) <= 0) // 向服务端发送请求报文。
        {
            perror("send");
            break;
        }
        printf("发送：%s\n", buffer.c_str());

        if ((iret = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0)) <= 0) // 接收服务端的回应报文。
        {
            printf("iret=%d\n", iret);
            break;
        }
        auto ret = yi::Parse(recv_buffer);
        player_move_call_back(ret.function_ret());
    }

    // 第4步：关闭socket，释放资源。
    close(sockfd);
}
