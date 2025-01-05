
// player_run.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesRun::PlayerStatesRun(Player* player)
    : player(player)
{
}

void
PlayerStatesRun::On_enter()
{
    player->object_color = COLOR_BLUE;
}

void
PlayerStatesRun::On_update(float delta_time)
{
    if(player->movement_velocity.approx_zero())
    {
        // 如果速度为0，切换到idle状态
        player->state_machine.Switch_to(PLAYER_STATE_IDLE);
    }
    else if(player->try_jump)
    {
        // 如果尝试跳跃，切换到jump状态
        player->state_machine.Switch_to(PLAYER_STATE_JUMP);
        player->try_jump = false;
    }
}

void
PlayerStatesRun::On_exit()
{
}
