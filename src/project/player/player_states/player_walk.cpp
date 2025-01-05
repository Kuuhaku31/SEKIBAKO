
// player_walk.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesWalk::PlayerStatesWalk(Player& player)
    : StateNode(PLAYER_STATE_WALK)
    , player(player)
{
}

void
PlayerStatesWalk::On_enter()
{
}

void
PlayerStatesWalk::On_update(float delta_time)
{
}

void
PlayerStatesWalk::On_exit()
{
}