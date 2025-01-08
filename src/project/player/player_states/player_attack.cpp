
// player_attack.cpp

#include "player.h"
#include "player_states.h"

#include "collision_manager.h"
#include "effect_master.h"
#include "imgui_setup.h"
#include "resources_name.h"
#include "resources_pool.h"

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

    // 攻击动作计时器初始化
    attack_action_timer.is_one_shot = true;
    attack_action_timer.Set_on_timeout(timer_callback);

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
}

void
PlayerStatesAttack::On_enter()
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();
    static EffectMaster&  effect_master  = EffectMaster::Instance();

    player.object_color = PLAYER_ATTACK_COLOR;

    player.is_Lock_facing_dir = true;
    player.is_lock_action_dir = true;

    player.attack_cd_done = false;
    player.attack_cd_timer.Set_wait_time(player.attack_cd);
    player.attack_cd_timer.Restart();

    attack_action_timer.Set_wait_time(player.attack_action_time);
    attack_effect_wait_timer.Set_wait_time(player.attack_effect_wait_time);
    attack_effect_timer.Set_wait_time(player.attack_effect_time);

    attack_action_timer.Restart();
    attack_effect_wait_timer.Restart();

    // 创建攻击碰撞盒
    attack_box    = collision_manager.Create_collision_box();
    attack_box->w = 2.0f;
    attack_box->h = 2.0f;

    // 开始攻击效果动画
    switch(player.action_dir)
    {
    case Player::Action_Dir::Up: current_attack_effect = effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_U)); break;
    case Player::Action_Dir::Down: current_attack_effect = effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_D)); break;
    case Player::Action_Dir::Left: current_attack_effect = effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_L)); break;
    default: // case Player::Action_Dir::Right:
    case Player::Action_Dir::Right: current_attack_effect = effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_R)); break;
    }

    attack_follow_player();
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
}

void
PlayerStatesAttack::On_update(float delta_time)
{
    // 更新计时器
    attack_action_timer.On_update(delta_time);
    attack_effect_wait_timer.On_update(delta_time);
    attack_effect_timer.On_update(delta_time);
}

void
PlayerStatesAttack::On_update_after(float delta_time)
{
    // 更新位置
    attack_follow_player();
}

void
PlayerStatesAttack::On_exit()
{
    player.is_Lock_facing_dir = false;
    player.is_lock_action_dir = false;

    // 销毁攻击碰撞盒
    collision_manager.Destroy_collision_box(attack_box);
}

void
PlayerStatesAttack::attack_follow_player()
{
    if(!current_attack_effect) return;

    switch(player.action_dir)
    {
    case Player::Action_Dir::Up:
    {
        attack_box->x = player.movement_position.vx - attack_box->w / 2; // 碰撞盒位置跟随角色
        attack_box->y = player.movement_position.vy - attack_box->h;

        float ph_x = player.movement_position.vx - current_attack_effect->Get_ph_w() / 2;
        float ph_y = player.movement_position.vy - current_attack_effect->Get_ph_h();

        current_attack_effect->Set_position_x(ph_x); // 攻击效果位置跟随角色
        current_attack_effect->Set_position_y(ph_y);

        break;
    }

    case Player::Action_Dir::Down:
    {
        attack_box->x = player.movement_position.vx - attack_box->w / 2;
        attack_box->y = player.movement_position.vy;

        float ph_x = player.movement_position.vx - current_attack_effect->Get_ph_w() / 2;
        float ph_y = player.movement_position.vy;

        current_attack_effect->Set_position_x(ph_x);
        current_attack_effect->Set_position_y(ph_y);

        break;
    }

    case Player::Action_Dir::Left:
    {
        attack_box->x = player.movement_position.vx - attack_box->w;
        attack_box->y = player.movement_position.vy - attack_box->h / 2;

        float ph_x = player.movement_position.vx - current_attack_effect->Get_ph_w();
        float ph_y = player.movement_position.vy - current_attack_effect->Get_ph_h() / 2;

        current_attack_effect->Set_position_x(ph_x);
        current_attack_effect->Set_position_y(ph_y);

        break;
    }

    default: //
    case Player::Action_Dir::Right:
    {
        attack_box->x = player.movement_position.vx;
        attack_box->y = player.movement_position.vy - attack_box->h / 2;

        float ph_x = player.movement_position.vx;
        float ph_y = player.movement_position.vy - current_attack_effect->Get_ph_h() / 2;

        current_attack_effect->Set_position_x(ph_x);
        current_attack_effect->Set_position_y(ph_y);

        break;
    }
    }
}
