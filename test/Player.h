#ifndef __YI_PLAYER__
#define __YI_PLAYER__
#include <cmath>
#include "../serialization/Request.pb.h"
namespace yi
{
    class Player
    {
    private:
        /* data */
        double x;
        double y;
        double speed;
        double aspect;
        double time;
    public:
        Player();
        Player(double x, double y, double speed, double aspect);
        ~Player();

        void move(double acceleration, double aspect, double time);
        yi::FunctionRet::PlayerMoveRet move(const yi::FunctionCall::PlayerMoveParams& func_call);

    };

    Player::Player()
    {
        x = 0;
        y = 0;
        speed = 0;
        aspect = 0;
        time = 0.5;
    }
    
    Player::Player(double x, double y, double speed, double aspect) : time(0.5)
    {
        this->x = x;
        this->y = y;
        this->speed = speed;
        this->aspect = aspect;
    }
    
    Player::~Player()
    {
    }


    void Player::move(double acceleration, double angle, double time)
    {
        // 根据现在的角度aspect和偏移的角度angle，结合当前的速度spedd和加速度acceleration以及运动的时间time
        // 计算出现在的x,y坐标
        // 计算出现在的速度speed
        this->x = x + speed * std::cos(angle) * time + acceleration * std::cos(angle) * time * time / 2;
        this->y = y + speed * std::sin(angle) * time + acceleration * std::sin(angle) * time * time / 2;
        this->speed = speed + acceleration * time;
        this->aspect = angle;
    }   

    yi::FunctionRet::PlayerMoveRet Player::move(const yi::FunctionCall::PlayerMoveParams& func_call)
    {
        yi::FunctionRet::PlayerMoveRet ret;
        this->x = func_call.x() + func_call.speed() * std::cos(func_call.angle()) * time + func_call.acceleration() * std::cos(func_call.angle()) * time * time / 2;
        this->y = func_call.y() + func_call.speed() * std::sin(func_call.angle()) * time + func_call.acceleration() * std::sin(func_call.angle()) * time * time / 2;
        this->speed = func_call.speed() + func_call.acceleration() * time;
        this->aspect = func_call.angle();
        ret.set_x(this->x);
        ret.set_y(this->y);
        ret.set_speed(this->speed);
        ret.set_aspect(this->aspect);

        return ret;
    }

    
}

#endif

