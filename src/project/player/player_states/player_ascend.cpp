
// player_ascend.cpp

#include "player_states.h"

#include "player.h"

PlayerStatesAscend::PlayerStatesAscend(Player& player)
    : StateNode(PLAYER_STATE_ASCEND)
    , player(player)
{
}

void
PlayerStatesAscend::On_enter()
{
}

void
PlayerStatesAscend::On_update(float delta_time)
{
}

void
PlayerStatesAscend::On_exit()
{
}
