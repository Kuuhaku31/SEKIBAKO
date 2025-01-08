
// player_roll.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesRoll::PlayerStatesRoll(Player& player)
    : StateNode(PLAYER_STATE_ROLL)
    , player(player)
{
    static Callback timer_callback = [&player]() {
        // 退出翻滚状态
        if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_PRESS_DASH) &&
            player.Is_try_move_x_on_one_dir() &&
            (player.movement_velocity.vx > player.dash_min_speed || player.movement_velocity.vx < -player.dash_min_speed))
        {
            // 1. 如果按住冲刺键
            // 2. 且有水平速度
            // 3. 且当前速度大于最小冲刺速度
            // 切换到 dash 状态
            player.Switch_to_state(PLAYER_STATE_DASH);
        }
        else
        {
            if(player.movement_velocity.vy) // 如果有竖直速度
            {
                player.Switch_to_state(PLAYER_STATE_LEVIATE);
            }
            else if(player.movement_velocity.vx) // 如果速度不为0
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
    };

    roll_timer.is_one_shot = true;
    roll_timer.Set_on_timeout(timer_callback); // 翻滚计时结束
}

void
PlayerStatesRoll::On_enter()
{
    player.object_color = PLAYER_ROLL_COLOR;

    player.roll_cd_done       = false;
    player.is_Lock_facing_dir = true;

    roll_timer.Set_wait_time(player.roll_time);
    roll_timer.Restart();

    player.roll_cd_timer.Set_wait_time(player.roll_cd);
    player.roll_cd_timer.Restart();
}

void
PlayerStatesRoll::On_update(float delta_time)
{
    roll_timer.On_update(delta_time);

    player.movement_acceleration += (player.Get_facing_vector() * player.roll_acceleration);
}

void
PlayerStatesRoll::On_exit()
{
    player.is_Lock_facing_dir = false;
}
