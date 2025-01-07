
// player.h

#pragma once

#include "base.h"
#include "state_machine.h"

/*
    idle    ->  | X         | walk      | run       | leviate   | jump      | roll      | X
    walk    ->  | idle      | X         | run       | leviate   | jump      | roll      | X
    run     ->  | idle      | walk      | X         | leviate   | jump      | roll      | X
    leviate ->  | idle      | walk      | run       | X         | jump      | X         | X
    jump    ->  | X         | X         | X         | leviate   | X         | X         | X
    roll    ->  | idle      | walk      | run       | X         | X         | X         | dash
    dash    ->  | X         | walk      | run       | X         | X         | X         | X
*/

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
#define PLAYER_CONTROL_PRESS_L_ALT PlayerControler(1 << 7)

#define CONTROLER_GET(controler, flag) (controler & flag)
#define CONTROLER_TRUE(controler, flag) (controler |= flag)
#define CONTROLER_FALSE(controler, flag) (controler &= ~flag)
#define CONTROLER_SET(controler, flag, value) (value ? controler |= flag : controler &= ~flag)

class Player : public Object, public StateMachine
{
    friend class Debuger;

    friend class PlayerStatesIdle;
    friend class PlayerStatesWalk;
    friend class PlayerStatesRun;
    friend class PlayerStatesLeviate;
    friend class PlayerStatesJump;
    friend class PlayerStatesRoll;
    friend class PlayerStatesDash;

    enum class PalyerFacing
    {
        Player_Facing_Left,
        Player_Facing_Right
    };

public:
    Player();
    ~Player() = default;

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    bool Is_try_move_x() const;            // 角色是否至少按下一个水平移动键
    bool Is_try_move_x_on_one_dir() const; // 角色是否只按下一个水平移动键
    bool Is_back_to_velocity() const;      // 角色面朝方向是否背对速度
    bool Is_try_walk() const;              // 角色是否尝试行走
    bool Is_try_run() const;               // 角色是否尝试奔跑

    const Vector2& Get_try_move_dir() const;  // 获取角色尝试移动的方向
    const Vector2& Get_facing_vector() const; // 获取角色面朝方向

public:
    PlayerControler player_controler = 0; // 角色控制器

    void On_stop_move(); // 停止移动

private:
    // 朝向
    PalyerFacing facing         = PalyerFacing::Player_Facing_Right; // 朝向
    bool         is_Lock_facing = false;                             // 是否锁定朝向

    // 移动
    float current_move_acceleration = 0.0f;  // 当前移动加速度
    float walk_acceleration         = 29.0f; // 行走加速度
    float run_acceleration          = 30.0f; // 跑步加速度

    // 阻力
    float player_friction            = 20.0f; // 角色摩擦力
    float player_air_resistance      = 0.1f;  // 角色空气阻力
    float player_friction_walk       = 20.0f; // 角色行走摩擦力
    float player_air_resistance_walk = 1.0f;  // 角色行走空气阻力

    // 跳跃
    float   jump_acceleration  = 250.0f; // 跳跃加速度
    float   jump_time          = 0.1f;   // 跳跃时间
    uint8_t can_jump_count     = 0;      // 当前可跳跃次数
    uint8_t can_jump_count_max = 2;      // 可跳跃次数

    // 翻滚
    float roll_acceleration = 100.0f; // 翻滚加速度
    float roll_time         = 0.2f;   // 翻滚时间
    float roll_cd           = 0.5f;   // 翻滚冷却
    bool  roll_cd_done      = true;   // 翻滚冷却是否完成
    Timer roll_cd_timer;              // 翻滚冷却计时器

    // 冲刺
    float dash_acceleration = 50.0f; // 冲刺加速度
    float dash_min_speed    = 20.0f; // 冲刺最小速度

    // 状态
    bool is_on_ground   = false; // 是否在地面上
    bool enable_gravity = true;  // 是否启用重力

    // 物理
    float GRAVITY         = 80.0f; // 重力加速度
    float floor_y         = 0.0f;  // 地面 y 坐标
    float floor_correct_y = 0.0f;  // 地面修正 y 坐标
};
