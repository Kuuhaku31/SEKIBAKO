
// palyer_idle.cpp

#include "player.h"
#include "player_states.h"

#include "resources_name.h"
#include "resources_pool.h"

PlayerStatesIdle::PlayerStatesIdle(Player& player)
    : StateNode(PLAYER_STATE_IDLE)
    , player(player)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    // 初始化 idle 动画
    player_idel = new AnimationInstance(*resources_pool.Get_animation(Ani_SEKIBAKO_idel_R));
}

PlayerStatesIdle::~PlayerStatesIdle()
{
    delete player_idel;
}

void
PlayerStatesIdle::On_enter()
{
    player.object_color = PLAYER_IDEEL_COLOR;

    player.is_on_ground   = true;
    player.enable_gravity = false;

    player.is_use_friction         = true;
    player.is_use_air_resistance   = true;
    player.movement_friction       = player.player_friction;
    player.movement_air_resistance = player.player_air_resistance;

    player.current_move_acceleration = player.run_acceleration;
    player.On_stop_move();

    player_idel->Restart();
}

void
PlayerStatesIdle::On_render() const
{
    player_idel->On_render();
}

void
PlayerStatesIdle::On_update(float delta_time)
{
    if(player.Is_try_walk() && player.movement_velocity.vx)
    {
        // 1. 如果尝试行走
        // 2. 且有水平速度
        // 切换到 walk 状态
        player.Switch_to_state(PLAYER_STATE_WALK);
    }
    else if(player.Is_try_run() && player.movement_velocity.vx)
    {
        // 1. 如果尝试奔跑
        // 2. 且有水平速度
        // 切换到 run 状态
        player.Switch_to_state(PLAYER_STATE_RUN);
    }
    else if(player.movement_velocity.vy)
    {
        // 如果有垂直速度
        // 切换到 leviate 状态
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 1. 如果尝试跳跃
        // 2. 且可以跳跃
        // 切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_DASH) && player.roll_cd_done)
    {
        // 1. 如果尝试翻滚
        // 2. 且 CD 完成
        // 切换到 roll 状态
        player.Switch_to_state(PLAYER_STATE_ROLL);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_ARROW) && player.attack_cd_done)
    {
        // 1. 如果尝试攻击
        // 2. 且 CD 完成
        // 切换到 attack 状态
        player.Switch_to_state(PLAYER_STATE_ATTACK);
    }
}

void
PlayerStatesIdle::On_update_after(float delta_time)
{
    // 角色动画更新
    player_idel->vx = player.movement_position.vx - player_idel->Get_ph_w() / 4;
    player_idel->vy = player.movement_position.vy - player_idel->Get_ph_h() + player.object_radius;

    player_idel->On_update(delta_time);
}

void
PlayerStatesIdle::On_exit()
{
}
