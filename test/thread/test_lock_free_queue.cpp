#include "LockFreeThreadPool.h"
#include "ThreadPool.h"
#include "../Clock.h"
#include <vector>
#include <thread>
#include <algorithm>
#include <future>
#include <memory>
#include <mutex>
#include "Singleton.h"
#include "../serialization/Parser.h"
#include "../Player.h"
void func()
{
    std::vector<int> v{5,4,3,2,1};
    std::sort(v.begin(), v.end());
}


int add(int a, int b)
{
    return a + b;
}


yi::FunctionRet player_move(const yi::FunctionCall& func_call)
{
    yi::FunctionRet ret;
    yi::FunctionCall::PlayerMoveParams params = func_call.player_move_params();
    yi::Player palyer;
    // 逻辑计算
    auto ret_params =  palyer.move(params); 
    // 设置好返回值的属性
    ret.set_function_name("player_move");
    ret.mutable_player_move_ret_params()->CopyFrom(ret_params);
    return ret;
}

void player_move_call_back(const yi::FunctionRet& ret)
{
    yi::FunctionRet::PlayerMoveRet player_move_ret = ret.player_move_ret_params();
    std::cout << "player_move_call_back: " << player_move_ret.x() << " " << player_move_ret.y() << " " << player_move_ret.speed() << " " << player_move_ret.aspect() << std::endl;
}

int main(int argc, char const *argv[])
{

    {
        yi::Clock clock;
        for (int i = 0; i<10000; ++i)
            func();
    }

    {
        std::vector<std::thread> threads;
        yi::Clock clock;
        for (int i = 0; i< 4; ++i)
        {
            threads.emplace_back([](){
                for (int i = 0; i<2500; ++i)
                    func();
            });
        }

        for (auto& t : threads)
            t.join();
    }


    static_assert(yi::Request_RequestType_ConnectionReq == yi::Request::ConnectionReq);
    

    // Singleton<thread::ThreadPool>::New(4);
    // {
    //     std::mutex mu;
    //     std::vector<std::future<int>> futures;
    //     yi::Clock clock;
    //     for (int i = 0; i < 4; i++)
    //     {
    //         std::async(std::launch::async, [&mu, &futures](){
    //             for (int i = 0; i<2000; ++i)
    //             {
    //                 std::unique_lock<std::mutex> lock(mu);
    //                 futures.push_back(Singleton<thread::ThreadPool>::Get()->enqueue(add, 1, 1));
    //             }
    //         });
    //     }
    //     for (auto& f : futures)
    //     {
    //         f.wait();
    //     }
    //     // for (auto& f : futures)
    //     // {
    //     //     std::cout << f.get() << std::endl;
    //     // }
    // }
    // Singleton<thread::ThreadPool>::Delete();

    yi::Singleton<yi::LockFreeThreadPool>::New(4, 1024);
    {
        std::vector<std::future<yi::FunctionRet>> futures;
        yi::FunctionCall func_call;
        func_call.set_function_name("player_move");
        YI_SET_FUNCTIONCALL_PARAMS(PlayerMove, player_move_params, x, 1.0, y, 2.0, acceleration, 3.0, speed, 4.0, angle, 5.0, aspect, 6.0)
        func_call.mutable_player_move_params()->CopyFrom(player_move_params);
        yi::Clock clock;
        for (int i = 0; i < 1000; i++)
        {
            futures.push_back(yi::Singleton<yi::LockFreeThreadPool>::Get()->enqueue(player_move, func_call));
        }
        for (auto& f : futures)
        {
            f.wait();
        }
        for (auto& f : futures)
        {
            player_move_call_back(f.get());
        }
    }
    yi::Singleton<yi::LockFreeThreadPool>::Delete();


    
}
