
// player.cpp

#include "player.h"

#include "player_states.h"

Player::Player()
    : Object(10.0f, 1.0f, COLOR_RED)
{
    state_machine.Register_state(PLAYER_STATE_IDLE, new PlayerStatesIdle(this));

    state_machine.Switch_to(PLAYER_STATE_IDLE);
}

void
Player::On_render() const
{
    Object::On_render();
}

void
Player::On_update(float delta_time)
{
    state_machine.On_update(delta_time);

    Object::On_update(delta_time);
}
