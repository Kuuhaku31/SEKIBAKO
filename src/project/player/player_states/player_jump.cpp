
// player_jump.cpp

#include "player_states.h"

#include "player.h"

PlayerStatesJump::PlayerStatesJump(Player& player)
    : StateNode(PLAYER_STATE_JUMP)
    , player(player)
{
}

void
PlayerStatesJump::On_enter()
{
}

void
PlayerStatesJump::On_update(float delta_time)
{
}

void
PlayerStatesJump::On_exit()
{
}
