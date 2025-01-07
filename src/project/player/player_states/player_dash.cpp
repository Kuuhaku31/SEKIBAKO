
// player_dash.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesDash::PlayerStatesDash(Player& player)
    : StateNode(PLAYER_STATE_DASH)
    , player(player)
{
}

void
PlayerStatesDash::On_enter()
{
    player.object_color = PLAYER_DASH_COLOR;

    player.is_Lock_facing = true;
}

void
PlayerStatesDash::On_update(float delta_time)
{
    if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_PRESS_DASH) &&
        player.Is_try_move_x_on_one_dir() &&
        (player.movement_velocity.vx > player.dash_min_speed || player.movement_velocity.vx < -player.dash_min_speed)

    )
    {
        // 1.只有在冲刺键按下
        // 2.并且只按下一个水平移动键
        // 3.并且当前速度大于最小冲刺速度
        // 时，才能冲刺
        player.movement_acceleration += (player.Get_facing_vector() * player.dash_acceleration);
    }
    else // 否则退出冲刺状态
    {
        // 如果速度不为0
        if(player.movement_velocity.vx)
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
            player.Switch_to_state(PLAYER_STATE_IDLE);
        }
    }
}

void
PlayerStatesDash::On_exit()
{
    player.is_Lock_facing = false;
}
