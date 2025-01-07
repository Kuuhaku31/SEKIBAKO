
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
    if(player.movement_position.vy >= player.floor_correct_y)
    {
        // 如果接触到地面，必须退出 leviate 状态
        if(player.movement_velocity.vx) // 如果速度不为0
        {
            if(player.Is_try_walk())
            {
                // 如果尝试行走，切换到 walk 状态
                player.Switch_to_state(PLAYER_STATE_WALK);
            }
            else // (player.Is_try_run())
            {
                // 如果尝试奔跑，切换到 run 状态
                player.Switch_to_state(PLAYER_STATE_RUN);
            }
        }
        else
        {
            // 如果速度为0，切换到 idle 状态
            player.Switch_to_state(PLAYER_STATE_IDLE);
        }
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 1. 如果尝试跳跃
        // 2. 且可以跳跃
        // 切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_ARROW) && player.attack_cd_done)
    {
        // 1. 如果尝试攻击
        // 2. 且 CD 完成
        // 切换到 attack 状态
        player.Switch_to_state(PLAYER_STATE_ATTACK);
    }
}


void
PlayerStatesLeviate::On_exit()
{
}
