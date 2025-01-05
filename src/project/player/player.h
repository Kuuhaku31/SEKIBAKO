
// player.h

#pragma once

#include "base.h"
#include "state_machine.h"

/*
    idle -> run, leviate, jump, roll
    run -> idle, leviate, jump, roll
    leviate -> idle, run, jump
    jump -> leviate
    roll -> idle, run
*/

enum class PalyerFacing
{
    Player_Facing_Left,
    Player_Facing_Right
};

class Player : public Object, public StateMachine
{
    friend class Debuger;

    friend class PlayerStatesIdle;
    friend class PlayerStatesRun;
    friend class PlayerStatesLeviate;
    friend class PlayerStatesJump;
    friend class PlayerStatesRoll;

public:
    Player();
    ~Player() = default;

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    bool try_jump = false;
    bool try_roll = false;

public:
    void On_stop_move() { movement_acceleration.to_zero(), movement_velocity.to_zero(); }

    void Set_facing(PalyerFacing facing) { this->facing = facing; }

private:
    PalyerFacing facing = PalyerFacing::Player_Facing_Right;

    float   jump_force = 250.0f;
    float   jump_time  = 0.1f;
    float   jump_cd    = 0.5f;
    uint8_t can_jump   = 2;

    float roll_force = 100.0f;
    float roll_time  = 0.2f;
    float roll_cd    = 0.5f;
    bool  can_roll   = true;
    Timer roll_cd_timer; // 翻滚冷却计时器

    bool is_on_ground = false; // 是否在地面上

    bool enable_gravity = true; // 是否启用重力

    const float GRAVITY = 80.0f; // 重力加速度

    float floor_y = 0; // 地面 y 坐标

    float floor_correct_y = floor_y - object_radius; // 地面修正 y 坐标
};
