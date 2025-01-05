
// player_jump.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesJump::PlayerStatesJump(Player& player)
    : StateNode(PLAYER_STATE_JUMP)
    , player(player)
{
    jump_timer.set_one_shot(true);
    jump_timer.set_on_timeout([&player]() {
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }); // 跳跃计时结束，切换到 leviate 状态
}

void
PlayerStatesJump::On_enter()
{
    player.object_color = COLOR_PURPLE;

    player.try_jump = false;

    player.is_on_ground   = false;
    player.enable_gravity = true;

    player.is_use_friction       = false;
    player.is_use_air_resistance = true;

    jump_timer.set_wait_time(player.jump_time);
    jump_timer.restart();
}

void
PlayerStatesJump::On_update(float delta_time)
{
    jump_timer.on_update(delta_time);
    player.movement_acceleration.vy -= player.jump_force;
}

void
PlayerStatesJump::On_exit()
{
}
