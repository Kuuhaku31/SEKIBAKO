
// player_run.cpp

#include "player.h"
#include "player_states.h"

#include "resources_name.h"
#include "resources_pool.h"

PlayerStatesRun::PlayerStatesRun(Player& player)
    : StateNode(PLAYER_STATE_RUN)
    , player(player)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    player_run = new AnimationInstance(*resources_pool.Get_animation(Ani_SEKIBAKO_run_R));
}

PlayerStatesRun::~PlayerStatesRun()
{
    delete player_run;
}

void
PlayerStatesRun::On_enter()
{
    player.object_color = PLAYER_RUN_COLOR;

    player.is_on_ground   = true;
    player.enable_gravity = false;

    player.is_use_friction       = true;
    player.is_use_air_resistance = true;

    player.movement_acceleration.vy = player.movement_velocity.vy = 0;

    player_run->Reset();
}

void
PlayerStatesRun::On_render() const
{
    player_run->On_render();
}

void
PlayerStatesRun::On_update(float delta_time)
{
    if(!player.Is_try_move_x_on_one_dir() && !player.movement_velocity.vx)
    {
        // 且
        // 1. 没有按下水平移动键
        // 2. 且速度为0
        // 切换到 idle 状态
        player.Switch_to_state(PLAYER_STATE_IDLE);
    }
    else if(player.Is_try_walk())
    {
        // 如果尝试行走，切换到 walk 状态
        player.Switch_to_state(PLAYER_STATE_WALK);
    }
    else if(player.movement_velocity.vy)
    {
        // 如果有垂直速度，切换到 leviate 状态
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 如果尝试跳跃，切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_DASH) &&
            player.roll_cd_done &&
            !player.Is_back_to_velocity())
    {
        // 1. 尝试翻滚
        // 2. CD 完成
        // 3. 面朝方向不背对速度
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
PlayerStatesRun::On_update_after(float delta_time)
{
    player_run->On_update(delta_time);
    player_run->vx = player.movement_position.vx - player_run->Get_ph_w() / 2;
    player_run->vy = player.movement_position.vy - player_run->Get_ph_h() + player.object_radius;
}

void
PlayerStatesRun::On_exit()
{
}
