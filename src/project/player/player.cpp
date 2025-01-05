
// player.cpp

#include "player.h"
#include "player_states.h"

#include "imgui_setup.h"

Player::Player()
    : Object(10.0f, 1.0f, COLOR_RED)
{
    // 创建状态
    static PlayerStatesIdle    state_idel(*this);
    static PlayerStatesRun     state_run(*this);
    static PlayerStatesLeviate state_leviate(*this);
    static PlayerStatesJump    state_jump(*this);
    static PlayerStatesRoll    state_roll(*this);

    // 注册状态
    Register_state(&state_idel);
    Register_state(&state_run);
    Register_state(&state_leviate);
    Register_state(&state_jump);
    Register_state(&state_roll);

    Switch_to_state(PLAYER_STATE_IDLE);

    // 计时器
    roll_cd_timer.set_one_shot(true);
    roll_cd_timer.set_on_timeout([&]() { can_roll = true; }); // 翻滚冷却计时结束
}

void
Player::On_render() const
{
    static const Painter& painter = Painter::Instance();

    painter.DrawCircle(
        movement_position.vx,
        movement_position.vy,
        object_radius,
        object_color,
        true

    );

    switch(facing)
    {
    case PalyerFacing::Player_Facing_Left:
        painter.DrawLine(
            movement_position.vx,
            movement_position.vy,
            movement_position.vx - 1,
            movement_position.vy,
            COLOR_YELLOW);
        break;

    case PalyerFacing::Player_Facing_Right:
        painter.DrawLine(
            movement_position.vx,
            movement_position.vy,
            movement_position.vx + 1,
            movement_position.vy,
            COLOR_YELLOW);
        break;
    }
}

void
Player::On_update(float delta_time)
{
    // 地面修正 y 坐标
    floor_correct_y = floor_y - object_radius;

    // 重置跳跃次数
    if(is_on_ground) can_jump = 2;

    // 更新状态机
    StateMachine::On_update(delta_time);

    // 计算重力
    if(enable_gravity) movement_acceleration.vy += GRAVITY;

    // 更新物理
    Object::On_update(delta_time);

    // 修正位置
    if(movement_position.vy > floor_correct_y) movement_velocity.vy = movement_position.vy = floor_correct_y;

    // 更新计时器
    roll_cd_timer.on_update(delta_time);
}
