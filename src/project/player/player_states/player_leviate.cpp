
// player_leviate.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesLeviate::PlayerStatesLeviate(Player& player)
    : StateNode(PLAYER_STATE_LEVIATE)
    , player(player)
{
}

void
PlayerStatesLeviate::On_enter()
{
    player.object_color = PLAYER_LEVIATE_COLOR;

    player.is_on_ground   = false;
    player.enable_gravity = true;

    player.is_use_friction       = false;
    player.is_use_air_resistance = true;
}

void
PlayerStatesLeviate::On_update(float delta_time)
{
    if(player.movement_position.vy >= player.floor_correct_y) // 如果碰到地面
    {
        if(player.movement_velocity.vx)
        {
            // 如果速度不为0，切换到 run 状态
            player.Switch_to_state(PLAYER_STATE_RUN);
        }
        else
        {
            // 如果速度为0，切换到 idle 状态
            player.Switch_to_state(PLAYER_STATE_IDLE);
        }
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 如果尝试跳跃，切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
}

void
PlayerStatesLeviate::On_exit()
{
}
