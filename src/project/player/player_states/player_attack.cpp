
// player_attack.cpp

#include "player.h"
#include "player_states.h"

#include "collision_manager.h"
#include "effect_master.h"
#include "imgui_setup.h"
#include "player_effects.h"
#include "resources_name.h"
#include "resources_pool.h"

static ResourcesPool&    resources_pool    = ResourcesPool::Instance();
static CollisionManager& collision_manager = CollisionManager::Instance();

PlayerStatesAttack::PlayerStatesAttack(Player& player)
    : StateNode(PLAYER_STATE_ATTACK)
    , player(player)
{
    static Callback timer_callback = [&player]() {
        // 攻击动作计时结束，退出攻击状态
        if(!player.is_on_ground) // 如果不在地面
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
    };

    // 动画
    player_attack = new AnimationInstance(*resources_pool.Get_animation(Ani_SEKIBAKO_attack_R));
    player_attack->Set_on_finished(timer_callback); // 攻击动作计时结束，退出攻击状态

    // 攻击效果等待计时器初始化
    attack_effect_wait_timer.is_one_shot = true;
    attack_effect_wait_timer.Set_on_timeout([&]() {
        attack_box->enable = true;
        attack_effect_timer.Restart();
    }); // 攻击效果等待计时结束，有效攻击碰撞盒

    // 攻击效果计时器初始化
    attack_effect_timer.is_one_shot = true;
    attack_effect_timer.Set_on_timeout([&]() {
        attack_box->enable = false;
    }); // 攻击效果计时结束，无效攻击碰撞盒
}

PlayerStatesAttack::~PlayerStatesAttack()
{
    collision_manager.Destroy_collision_box(attack_box);
    delete player_attack; // 释放攻击动画
}

void
PlayerStatesAttack::On_enter()
{
    static EffectMaster& effect_master = EffectMaster::Instance();

    player.object_color = PLAYER_ATTACK_COLOR;

    player.is_Lock_facing_dir = true;
    player.is_lock_action_dir = true;

    player.attack_cd_done = false;
    player.attack_cd_timer.Set_wait_time(player.attack_cd);
    player.attack_cd_timer.Restart();

    // attack_action_timer.Set_wait_time(player.attack_action_time);
    attack_effect_wait_timer.Set_wait_time(player.attack_effect_wait_time);
    attack_effect_timer.Set_wait_time(player.attack_effect_time);

    // attack_action_timer.Restart();
    player_attack->Reset();
    attack_effect_wait_timer.Restart();

    // 创建攻击碰撞盒
    attack_box    = collision_manager.Create_collision_box();
    attack_box->w = 2.0f;
    attack_box->h = 2.0f;

    // 注册攻击效果动画
    uint8_t attack_dir = 0;
    switch(player.action_dir)
    {
    case Player::Action_Dir::Up: attack_dir = 0; break;
    case Player::Action_Dir::Down: attack_dir = 1; break;
    case Player::Action_Dir::Left: attack_dir = 2; break;
    default:
    case Player::Action_Dir::Right: attack_dir = 3; break;
    }

    PlayerAttackEffect* attack_effect = new PlayerAttackEffect(player.movement_position, attack_dir);

    Vector2* effect_pos_corr = &attack_effect->attack_effect_animation.position_correction;
    float    effect_ph_w     = attack_effect->attack_effect_animation.Get_ph_w();
    float    effect_ph_h     = attack_effect->attack_effect_animation.Get_ph_h();

    // 设置位置修正
    switch(player.action_dir)
    {
    case Player::Action_Dir::Up:
    {
        effect_pos_corr->vx = player.movement_position.vx - effect_ph_w / 2;
        effect_pos_corr->vy = player.movement_position.vy - effect_ph_h;
        break;
    }
    case Player::Action_Dir::Down:
    {
        effect_pos_corr->vx = player.movement_position.vx - effect_ph_w / 2;
        effect_pos_corr->vy = player.movement_position.vy;
        break;
    }
    case Player::Action_Dir::Left:
    {
        effect_pos_corr->vx = player.movement_position.vx - effect_ph_w;
        effect_pos_corr->vy = player.movement_position.vy - effect_ph_h / 2;
        break;
    }
    default:
    case Player::Action_Dir::Right:
    {
        effect_pos_corr->vx = player.movement_position.vx;
        effect_pos_corr->vy = player.movement_position.vy - effect_ph_h / 2;
        break;
    }
    }

    effect_master.Register_effect(attack_effect);
}

void
PlayerStatesAttack::On_render() const
{
    // 绘制攻击碰撞盒
    static const Painter& painter = Painter::Instance();
    painter.DrawRect(
        attack_box->x,
        attack_box->y,
        attack_box->w,
        attack_box->h,
        attack_box->enable ? COLOR_RED : COLOR_WHITE,
        true

    );

    player_attack->On_render();
}

void
PlayerStatesAttack::On_update(float delta_time)
{
    // 更新计时器
    // attack_action_timer.On_update(delta_time);
    player_attack->On_update(delta_time);
    attack_effect_wait_timer.On_update(delta_time);
    attack_effect_timer.On_update(delta_time);
}

void
PlayerStatesAttack::On_update_after(float delta_time)
{
    // 更新位置
    player_attack->vx = player.movement_position.vx - player_attack->Get_ph_w() / 2;
    player_attack->vy = player.movement_position.vy - player_attack->Get_ph_h() + player.object_radius;

    // 更新攻击碰撞盒位置
    attack_box->x = player.movement_position.vx - attack_box->w / 2;
    attack_box->y = player.movement_position.vy - attack_box->h / 2;
}

void
PlayerStatesAttack::On_exit()
{
    player.is_Lock_facing_dir = false;
    player.is_lock_action_dir = false;

    // 销毁攻击碰撞盒
    collision_manager.Destroy_collision_box(attack_box);
}
