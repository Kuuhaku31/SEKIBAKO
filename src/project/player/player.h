
// player.h

#pragma once

#include "base.h"

enum class PlayerState
{
    Player_Idle,      // 空闲
    Player_Walking,   // 行走
    Player_Running,   // 奔跑
    Player_Jumping,   // 跳跃
    Player_Ascending, // 上升
    Player_Falling,   // 下落
    Player_Attacking, // 攻击
    Player_Hurt,      // 受伤
    Player_Dead       // 死亡
};

enum class PalyerFacing
{
    Player_Facing_Left,
    Player_Facing_Right
};

class Player : public Object
{
public:
    Player();
    Player(const Vector2& position, Color color);
    ~Player() = default;

public:
private:
    PlayerState  state  = PlayerState::Player_Idle;
    PalyerFacing facing = PalyerFacing::Player_Facing_Right;

    float force          = 2000.0f;
    float friction       = 500.0f;
    float air_resistance = 0.01f;

    float jump_force = 4000.0f;
    float jump_time  = 0.5f;
};
