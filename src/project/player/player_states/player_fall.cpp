
// player_fall.cpp

#include "player_states.h"

#include "player.h"

PlayerStatesFall::PlayerStatesFall(Player& player)
    : StateNode(PLAYER_STATE_FALL)
    , player(player)
{
}

void
PlayerStatesFall::On_enter()
{
}

void
PlayerStatesFall::On_update(float delta_time)
{
}

void
PlayerStatesFall::On_exit()
{
}
