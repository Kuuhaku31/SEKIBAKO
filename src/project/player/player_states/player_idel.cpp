
// palyer_idle.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesIdle::PlayerStatesIdle(Player* player)
    : player(player)
{
}

void
PlayerStatesIdle::On_enter()
{
    player->object_color = COLOR_GREEN;
}

void
PlayerStatesIdle::On_update(float delta_time)
{
    if(!player->movement_velocity.approx_zero())
    {
        // 如果速度不为0，切换到run状态
        player->state_machine.Switch_to(PLAYER_STATE_RUN);
    }
    else if(player->try_jump)
    {
        // 如果尝试跳跃，切换到jump状态
        player->state_machine.Switch_to(PLAYER_STATE_JUMP);
        player->try_jump = false;
    }
}

void
PlayerStatesIdle::On_exit()
{
}
