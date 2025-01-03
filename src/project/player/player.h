
// player.h

#pragma once

#include "base.h"

enum class PlayerState
{
    Player_Idle,   // 空闲
    Player_Walk,   // 行走
    Player_Run,    // 奔跑
    Player_Jump,   // 跳跃
    Player_Attack, // 攻击
    Player_Hurt,   // 受伤
    Player_Dead    // 死亡
};

class Player : public Object
{
};
