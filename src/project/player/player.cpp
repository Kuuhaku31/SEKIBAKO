
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

    // 注册状态
    Register_state(&state_idel);
    Register_state(&state_run);
    Register_state(&state_leviate);
    Register_state(&state_jump);

    Switch_to_state(PLAYER_STATE_IDLE);
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
    floor_correct_y = floor_y - object_radius;

    StateMachine::On_update(delta_time);

    if(enable_gravity) movement_acceleration.vy += GRAVITY;

    Object::On_update(delta_time);

    // 修正位置
    if(movement_position.vy > floor_correct_y) movement_velocity.vy = movement_position.vy = floor_correct_y;
}
