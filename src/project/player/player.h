
// player.h

#pragma once

#include "base.h"
#include "state_machine.h"

// 角色状态
#define PLAYER_STATE_IDLE "player_idle"     // 角色空闲
#define PLAYER_STATE_WALK "player_walk"     // 角色行走
#define PLAYER_STATE_RUN "player_run"       // 角色奔跑
#define PLAYER_STATE_JUMP "player_jump"     // 角色跳跃
#define PLAYER_STATE_ASCEND "player_ascend" // 角色上升
#define PLAYER_STATE_FALL "player_fall"     // 角色下落
#define PLAYER_STATE_ATTACK "player_attack" // 角色攻击
#define PLAYER_STATE_HURT "player_hurt"     // 角色受伤
#define PLAYER_STATE_DEAD "player_dead"     // 角色死亡

/*
    idle -> run, jump
    run -> idle, jump
    jump -> ascend, fall
    ascend -> jump, fall
    fall -> idle, run, jump
*/

enum class PalyerFacing
{
    Player_Facing_Left,
    Player_Facing_Right
};

class Player : public Object
{
    friend class PlayerStatesIdle;
    friend class PlayerStatesRun;

public:
    Player();
    ~Player() = default;

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    void On_jump() { try_jump = true; }

private:
    StateMachine state_machine;
    PalyerFacing facing = PalyerFacing::Player_Facing_Right;

    bool  try_jump   = false;
    float jump_force = 4000.0f;
    float jump_time  = 0.5f;

    bool enable_gravity = true; // 是否启用重力

    const float GRAVITY = 40.0f; // 重力加速度
};
