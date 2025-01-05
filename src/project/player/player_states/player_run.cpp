
// player_run.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesRun::PlayerStatesRun(Player& player)
    : StateNode(PLAYER_STATE_RUN)
    , player(player)
{
}

void
PlayerStatesRun::On_enter()
{
    player.object_color = COLOR_BLUE;

    player.is_on_ground   = true;
    player.enable_gravity = false;
}

void
PlayerStatesRun::On_update(float delta_time)
{
    if(!player.movement_velocity.vx)
    {
        // 如果速度为0，切换到 idle 状态
        player.Switch_to_state(PLAYER_STATE_IDLE);
    }
    else if(player.movement_velocity.vy)
    {
        // 如果有垂直速度，切换到 leviate 状态
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }
}

void
PlayerStatesRun::On_exit()
{
}
