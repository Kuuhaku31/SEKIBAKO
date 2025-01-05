
// player.cpp

#include "player.h"
#include "player_states.h"

#include "imgui_setup.h"

Player::Player()
    : Object(10.0f, 1.0f, COLOR_RED)
{
    state_machine.Register_state(PLAYER_STATE_IDLE, new PlayerStatesIdle(this));
    state_machine.Register_state(PLAYER_STATE_RUN, new PlayerStatesRun(this));

    state_machine.Switch_to(PLAYER_STATE_IDLE);
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
    state_machine.On_update(delta_time);

    Object::On_update(delta_time);
}
