
// player_leviate.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesLeviate::PlayerStatesLeviate(Player& player)
    : player(player)
{
}

void
PlayerStatesLeviate::On_enter()
{
    player.object_color = COLOR_YELLOW;

    player.is_on_ground   = false;
    player.enable_gravity = true;
}

void
PlayerStatesLeviate::On_update(float delta_time)
{
    // 如果碰到地面
    if(player.movement_position.vy >= player.floor_correct_y)
    {
        if(player.movement_velocity.vx)
        {
            // 如果速度不为0，切换到 run 状态
            player.state_machine.Switch_to(PLAYER_STATE_RUN);
        }
        else
        {
            // 如果速度为0，切换到 idle 状态
            player.state_machine.Switch_to(PLAYER_STATE_IDLE);
        }
    }
}

void
PlayerStatesLeviate::On_exit()
{
}
