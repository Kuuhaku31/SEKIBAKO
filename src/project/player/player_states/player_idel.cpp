
// palyer_idle.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesIdle::PlayerStatesIdle(Player& player)
    : StateNode(PLAYER_STATE_IDLE)
    , player(player)
{
}

void
PlayerStatesIdle::On_enter()
{
    player.object_color = COLOR_GREEN;

    player.is_on_ground   = true;
    player.enable_gravity = false;

    player.is_use_friction       = true;
    player.is_use_air_resistance = true;

    player.On_stop_move();
}

void
PlayerStatesIdle::On_update(float delta_time)
{
    if(player.movement_velocity.vx)
    {
        // 如果速度不为0，切换到 run 状态
        player.Switch_to_state(PLAYER_STATE_RUN);
    }
    else if(player.movement_velocity.vy)
    {
        // 如果有垂直速度，切换到 leviate 状态
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }
    else if(player.try_jump)
    {
        // 如果尝试跳跃，切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
}

void
PlayerStatesIdle::On_exit()
{
}