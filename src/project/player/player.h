
// player.h

#pragma once

#include "base.h"
#include "state_machine.h"

/*
    idle -> run, leviate, jump, roll
    run -> idle, leviate, jump, roll
    leviate -> idle, run, jump
    jump -> leviate
    roll -> idle, run, dash
    dash -> run
*/

enum class PalyerFacing
{
    Player_Facing_Left,
    Player_Facing_Right
};

typedef uint32_t PlayerControler;
// 冲刺键（按下） | 冲刺键（点击） | 跳跃键（点击） | 右移键（按下） | 左移键（按下） | 下移键（按下） | 上移键（按下） |
#define PLAYER_CONTROL_PRESS_UP PlayerControler(1 << 0)
#define PLAYER_CONTROL_PRESS_DOWN PlayerControler(1 << 1)
#define PLAYER_CONTROL_PRESS_LEFT PlayerControler(1 << 2)
#define PLAYER_CONTROL_PRESS_RIGHT PlayerControler(1 << 3)
#define PLAYER_CONTROL_IS_MOVE_X PlayerControler(PLAYER_CONTROL_PRESS_LEFT | PLAYER_CONTROL_PRESS_RIGHT)
#define PLAYER_CONTROL_CLICK_JUMP PlayerControler(1 << 4)
#define PLAYER_CONTROL_CLICK_DASH PlayerControler(1 << 5)
#define PLAYER_CONTROL_PRESS_DASH PlayerControler(1 << 6)

#define CONTROLER_GET(controler, flag) (controler & flag)
#define CONTROLER_TRUE(controler, flag) (controler |= flag)
#define CONTROLER_FALSE(controler, flag) (controler &= ~flag)
#define CONTROLER_SET(controler, flag, value) (value ? controler |= flag : controler &= ~flag)

class Player : public Object, public StateMachine
{
    friend class Debuger;

    friend class PlayerStatesIdle;
    friend class PlayerStatesRun;
    friend class PlayerStatesLeviate;
    friend class PlayerStatesJump;
    friend class PlayerStatesRoll;
    friend class PlayerStatesDash;

public:
    Player();
    ~Player() = default;

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    // 控制
    PlayerControler player_controler = 0;

    // 角色是否至少按下一个水平移动键
    bool Is_try_move_x() const
    {
        return player_controler & PLAYER_CONTROL_IS_MOVE_X;
    }

    // 角色是否只按下一个水平移动键
    bool Is_try_move_x_on_one_dir() const
    {
        return (player_controler & PLAYER_CONTROL_IS_MOVE_X) == PLAYER_CONTROL_PRESS_LEFT ||
               (player_controler & PLAYER_CONTROL_IS_MOVE_X) == PLAYER_CONTROL_PRESS_RIGHT;
    }

    // 角色面朝方向是否背对速度
    bool Is_back_to_velocity() const
    {
        switch(facing)
        {
        case PalyerFacing::Player_Facing_Left: return movement_velocity.vx > 0;
        case PalyerFacing::Player_Facing_Right: return movement_velocity.vx < 0;
        default: return false;
        }
    }

public:
    const Vector2& Get_facing_vector() const;

    void On_stop_move()
    {
        movement_acceleration.to_zero(), movement_velocity.to_zero();
    }

private:
    PalyerFacing facing = PalyerFacing::Player_Facing_Right;
    // 是否锁定朝向
    bool is_Lock_facing = false;

    // 跳跃
    float   jump_force = 250.0f; // 跳跃力
    float   jump_time  = 0.1f;   // 跳跃时间
    uint8_t can_jump   = 2;      // 可跳跃次数

    // 翻滚
    float roll_force   = 100.0f; // 翻滚力
    float roll_time    = 0.2f;   // 翻滚时间
    float roll_cd      = 0.5f;   // 翻滚冷却
    bool  roll_cd_done = true;   // 翻滚冷却是否完成
    Timer roll_cd_timer;         // 翻滚冷却计时器

    // 冲刺
    float dash_acceleration = 100.0f;
    float dash_min_speed    = 20.0f; // 冲刺最小速度


    bool is_on_ground = false; // 是否在地面上

    bool enable_gravity = true; // 是否启用重力

    const float GRAVITY = 80.0f; // 重力加速度

    float floor_y = 0; // 地面 y 坐标

    float floor_correct_y = floor_y - object_radius; // 地面修正 y 坐标
};
