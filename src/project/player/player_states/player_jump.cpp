
// player_jump.cpp

#include "player.h"
#include "player_states.h"

#include "effect_master.h"
#include "player_effects.h"
#include "resources_name.h"
#include "resources_pool.h"

PlayerStatesJump::PlayerStatesJump(Player& player)
    : StateNode(PLAYER_STATE_JUMP)
    , player(player)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    // 动画
    player_jump = new AnimationInstance(*resources_pool.Get_animation(Ani_SEKIBAKO_jump_R));
    player_jump->Set_on_finished([&player]() { player.Switch_to_state(PLAYER_STATE_LEVIATE); }); // 跳跃动画结束，切换到 leviate 状态
}

PlayerStatesJump::~PlayerStatesJump()
{
    delete player_jump;
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

    // 创建特效
    PlayerJumpEffect* jump_effect = new PlayerJumpEffect(player.movement_position);

    // 特效的动画
    AnimationInstance* jump_effect_ani = &jump_effect->jump_effect_animation;

    // 设置位置
    jump_effect_ani->vx = player.movement_position.vx - jump_effect_ani->Get_ph_w() / 2;
    jump_effect_ani->vy = player.movement_position.vy - jump_effect_ani->Get_ph_h() + player.object_radius;

    // 添加到特效管理器
    effect_master.Register_effect(jump_effect);

    player_jump->Set_play_time(player.jump_time);
    player_jump->Restart();
}

void
PlayerStatesJump::On_render() const
{
    player_jump->On_render();
}

void
PlayerStatesJump::On_update(float delta_time)
{
    player_jump->On_update(delta_time);

    player.movement_acceleration.vy -= player.jump_acceleration;
}

void
PlayerStatesJump::On_update_after(float delta_time)
{
    player_jump->vx = player.movement_position.vx - player_jump->Get_ph_w() / 2;
    player_jump->vy = player.movement_position.vy - player_jump->Get_ph_h();
}

void
PlayerStatesJump::On_exit()
{
}
