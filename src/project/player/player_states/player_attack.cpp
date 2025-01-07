
// player_attack.cpp

#include "player.h"
#include "player_states.h"

#include "collision_manager.h"
#include "imgui_setup.h"

static CollisionManager& collision_manager = CollisionManager::Instance();

PlayerStatesAttack::PlayerStatesAttack(Player& player)
    : StateNode(PLAYER_STATE_ATTACK)
    , player(player)
{
    // 攻击动作计时器初始化
    attack_action_timer.set_one_shot(true);
    attack_action_timer.set_on_timeout([&]() {
        player.Switch_to_state(PLAYER_STATE_IDLE);
    }); // 攻击动作计时结束，退出攻击状态

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
}

void
PlayerStatesAttack::On_enter()
{
    player.object_color = PLAYER_ATTACK_COLOR;

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
    attack_box_follow_player();
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
    attack_action_timer.on_update(delta_time);
    attack_effect_wait_timer.on_update(delta_time);
    attack_effect_timer.on_update(delta_time);

    // 更新位置
    attack_box_follow_player();
}

void
PlayerStatesAttack::On_exit()
{
    // 销毁攻击碰撞盒
    collision_manager.Destroy_collision_box(attack_box);
}

void
PlayerStatesAttack::attack_box_follow_player()
{
    attack_box->x = player.movement_position.vx;
    attack_box->y = player.movement_position.vy;
}
