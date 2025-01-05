
// player_roll.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesRoll::PlayerStatesRoll(Player& player)
    : StateNode(PLAYER_STATE_ROLL)
    , player(player)
{
    roll_timer.set_one_shot(true);
    roll_timer.set_on_timeout([&player]() {
        if(player.movement_velocity.vx)
        {
            player.Switch_to_state(PLAYER_STATE_RUN);
        }
        else
        {
            player.Switch_to_state(PLAYER_STATE_IDLE);
        }
    }); // 翻滚计时结束
}

void
PlayerStatesRoll::On_enter()
{
    player.object_color = COLOR_ORANGE;

    player.can_roll = false;

    roll_timer.set_wait_time(player.roll_time);
    roll_timer.restart();

    player.roll_cd_timer.set_wait_time(player.roll_cd);
    player.roll_cd_timer.restart();
}

void
PlayerStatesRoll::On_update(float delta_time)
{
    roll_timer.on_update(delta_time);

    switch(player.facing)
    {
    case PalyerFacing::Player_Facing_Left: player.movement_acceleration.vx -= player.roll_force; break;
    case PalyerFacing::Player_Facing_Right: player.movement_acceleration.vx += player.roll_force; break;
    default: break;
    }
}

void
PlayerStatesRoll::On_exit()
{
}
