
// player_attack.cpp

#include "player.h"
#include "player_states.h"

#include "animation.h"
#include "collision_manager.h"
#include "imgui_setup.h"

static CollisionManager& collision_manager = CollisionManager::Instance();

PlayerStatesAttack::PlayerStatesAttack(Player& player, const AnimationInfo* attack_effect_infos)
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
    attack_action_timer.set_one_shot(true);
    attack_action_timer.set_on_timeout(timer_callback);

    // 攻击效果等待计时器初始化
    attack_effect_wait_timer.set_one_shot(true);
    attack_effect_wait_timer.set_on_timeout([&]() {
        attack_box->enable = true;
        attack_effect_timer.restart();
    }); // 攻击效果等待计时结束，有效攻击碰撞盒

    // 攻击效果计时器初始化
    attack_effect_timer.set_one_shot(true);
    attack_effect_timer.set_on_timeout([&]() {
        attack_box->enable = false;
    }); // 攻击效果计时结束，无效攻击碰撞盒

    // 创建攻击效果动画
    for(int i = 0; i < 4; i++) attack_effect_list[i] = new Animation(attack_effect_infos[i]);
}

PlayerStatesAttack::~PlayerStatesAttack()
{
    collision_manager.Destroy_collision_box(attack_box);
    for(int i = 0; i < 4; i++) delete attack_effect_list[i];
}

void
PlayerStatesAttack::On_enter()
{
    player.object_color = PLAYER_ATTACK_COLOR;

    player.is_Lock_facing_dir = true;
    player.is_lock_action_dir = true;

    player.attack_cd_done = false;
    player.attack_cd_timer.set_wait_time(player.attack_cd);
    player.attack_cd_timer.restart();

    attack_action_timer.set_wait_time(player.attack_action_time);
    attack_effect_wait_timer.set_wait_time(player.attack_effect_wait_time);
    attack_effect_timer.set_wait_time(player.attack_effect_time);

    attack_action_timer.restart();
    attack_effect_wait_timer.restart();

    // 创建攻击碰撞盒
    attack_box    = collision_manager.Create_collision_box();
    attack_box->w = 2.0f;
    attack_box->h = 2.0f;

    // 开始攻击效果动画
    switch(player.action_dir)
    {
    case Player::Action_Dir::Up: current_attack_effect = attack_effect_list[0]; break;
    case Player::Action_Dir::Down: current_attack_effect = attack_effect_list[1]; break;
    case Player::Action_Dir::Left: current_attack_effect = attack_effect_list[2]; break;
    default: //
    case Player::Action_Dir::Right: current_attack_effect = attack_effect_list[3]; break;
    }
    current_attack_effect->Reset();

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

    // 绘制攻击效果
    current_attack_effect->On_render();
}

void
PlayerStatesAttack::On_update(float delta_time)
{
    // 更新计时器
    attack_action_timer.on_update(delta_time);
    attack_effect_wait_timer.on_update(delta_time);
    attack_effect_timer.on_update(delta_time);

    // 更新动画
    current_attack_effect->on_update(delta_time);
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
    switch(player.action_dir)
    {
    case Player::Action_Dir::Up:
    {
        attack_box->x = player.movement_position.vx - attack_box->w / 2; // 碰撞盒位置跟随角色
        attack_box->y = player.movement_position.vy - attack_box->h;

        current_attack_effect->x = player.movement_position.vx - current_attack_effect->w / 2; // 特效位置跟随角色
        current_attack_effect->y = player.movement_position.vy - current_attack_effect->h;

        break;
    }

    case Player::Action_Dir::Down:
    {
        attack_box->x = player.movement_position.vx - attack_box->w / 2;
        attack_box->y = player.movement_position.vy;

        current_attack_effect->x = player.movement_position.vx - current_attack_effect->w / 2;
        current_attack_effect->y = player.movement_position.vy;

        break;
    }

    case Player::Action_Dir::Left:
    {
        attack_box->x = player.movement_position.vx - attack_box->w;
        attack_box->y = player.movement_position.vy - attack_box->h / 2;

        current_attack_effect->x = player.movement_position.vx - current_attack_effect->w;
        current_attack_effect->y = player.movement_position.vy - current_attack_effect->h / 2;

        break;
    }

    default: //
    case Player::Action_Dir::Right:
    {
        attack_box->x = player.movement_position.vx;
        attack_box->y = player.movement_position.vy - attack_box->h / 2;

        current_attack_effect->x = player.movement_position.vx;
        current_attack_effect->y = player.movement_position.vy - current_attack_effect->h / 2;

        break;
    }
    }
}
