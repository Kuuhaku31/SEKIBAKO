
// player_walk.cpp

#include "player.h"
#include "player_states.h"

PlayerStatesWalk::PlayerStatesWalk(Player& player)
    : StateNode(PLAYER_STATE_WALK)
    , player(player)
{
}

void
PlayerStatesWalk::On_enter()
{
    player.object_color = PLAYER_WALK_COLOR;

    player.is_on_ground   = true;
    player.enable_gravity = false;

    player.is_use_friction         = true;
    player.is_use_air_resistance   = true;
    player.movement_friction       = player.player_friction_walk;
    player.movement_air_resistance = player.player_air_resistance_walk;

    player.current_move_acceleration = player.walk_acceleration;
    player.movement_acceleration.vy = player.movement_velocity.vy = 0;
}

void
PlayerStatesWalk::On_update(float delta_time)
{
    if(!player.Is_try_move_x_on_one_dir() && !player.movement_velocity.vx)
    {
        // 且
        // 1. 没有按下水平移动键
        // 2. 且速度为0
        // 切换到 idle 状态
        player.Switch_to_state(PLAYER_STATE_IDLE);
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
PlayerStatesWalk::On_exit()
{
    player.current_move_acceleration = player.run_acceleration;

    player.movement_friction       = player.player_friction;
    player.movement_air_resistance = player.player_air_resistance;
}
