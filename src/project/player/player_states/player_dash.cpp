
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
}

void
PlayerStatesDash::On_update(float delta_time)
{
    // 只有在冲刺键按下并且有水平速度时，才能冲刺
    if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_PRESS_DASH) &&
        CONTROLER_GET(player.player_controler, PLAYER_CONTROL_IS_MOVE_X))
    {
        player.movement_acceleration += (player.Get_facing_vector() * player.dash_acceleration);
    }
    else // 否则退出冲刺状态
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
}

void
PlayerStatesDash::On_exit()
{
}
