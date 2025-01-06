
// player_roll.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesRoll::PlayerStatesRoll(Player& player)
    : StateNode(PLAYER_STATE_ROLL)
    , player(player)
{
    roll_timer.set_one_shot(true);
    roll_timer.set_on_timeout([&player]() {
        // 退出翻滚状态
        if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_PRESS_DASH) &&
            player.Is_try_move_x_on_one_dir() &&
            (player.movement_velocity.vx > player.dash_min_speed || player.movement_velocity.vx < -player.dash_min_speed))
        {
            // 如果按住冲刺键
            // 并且有水平速度
            // 并且当前速度大于最小冲刺速度
            // 切换到 dash 状态
            player.Switch_to_state(PLAYER_STATE_DASH);
        }
        else
        {
            if(player.movement_velocity.vx)
            {
                player.Switch_to_state(PLAYER_STATE_RUN);
            }
            else
            {
                player.Switch_to_state(PLAYER_STATE_IDLE);
            }
        }
    }); // 翻滚计时结束
}

void
PlayerStatesRoll::On_enter()
{
    player.object_color = PLAYER_ROLL_COLOR;

    player.can_roll       = false;
    player.is_Lock_facing = true;

    roll_timer.set_wait_time(player.roll_time);
    roll_timer.restart();

    player.roll_cd_timer.set_wait_time(player.roll_cd);
    player.roll_cd_timer.restart();
}

void
PlayerStatesRoll::On_update(float delta_time)
{
    roll_timer.on_update(delta_time);

    player.movement_acceleration += (player.Get_facing_vector() * player.roll_force);
}

void
PlayerStatesRoll::On_exit()
{
    player.is_Lock_facing = false;
}
