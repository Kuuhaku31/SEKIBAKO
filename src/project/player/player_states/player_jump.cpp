
// player_jump.cpp

#include "player.h"
#include "player_states.h"

#include "animation.h"
#include "effect_master.h"

PlayerStatesJump::PlayerStatesJump(Player& player, const AnimationInfo& info)
    : StateNode(PLAYER_STATE_JUMP)
    , player(player)
{
    jump_timer.is_one_shot = true;
    jump_timer.Set_on_timeout([&player]() {
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }); // 跳跃计时结束，切换到 leviate 状态

    jump_effect_info = info;
}

PlayerStatesJump::~PlayerStatesJump()
{
}

void
PlayerStatesJump::On_enter()
{
    static EffectMaster& effect_master = EffectMaster::Instance();

    player.object_color = PLAYER_JUMP_COLOR;

    CONTROLER_FALSE(player.player_controler, PLAYER_CONTROL_CLICK_JUMP);
    player.can_jump_count--;

    if(player.movement_velocity.vy > 0) player.movement_velocity.vy = 0;

    player.is_on_ground   = false;
    player.enable_gravity = true;

    player.is_use_friction       = false;
    player.is_use_air_resistance = true;

    jump_timer.Set_wait_time(player.jump_time);
    jump_timer.Restart();

    // 跳跃效果
    Animation* jump_effect = effect_master.Create_effect(jump_effect_info);

    // 设置位置
    jump_effect->x = player.movement_position.vx - jump_effect->w / 2;
    jump_effect->y = player.movement_position.vy - jump_effect->h + player.object_radius;

    // 设置动画
    jump_effect->Animation_reset();
}

void
PlayerStatesJump::On_render() const
{
}

void
PlayerStatesJump::On_update(float delta_time)
{
    jump_timer.On_update(delta_time);
    player.movement_acceleration.vy -= player.jump_acceleration;
}

void
PlayerStatesJump::On_exit()
{
}
