
// player_dash.cpp

#include "player.h"
#include "player_states.h"

#include "resources_name.h"
#include "resources_pool.h"

PlayerStatesDash::PlayerStatesDash(Player& player)
    : StateNode(PLAYER_STATE_DASH)
    , player(player)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    player_dash = new AnimationInstance(*resources_pool.Get_animation(Ani_SEKIBAKO_roll_R));
}

PlayerStatesDash::~PlayerStatesDash()
{
    delete player_dash;
}

void
PlayerStatesDash::On_enter()
{
    player.object_color = PLAYER_DASH_COLOR;

    player.is_Lock_facing_dir = true;
}

void
PlayerStatesDash::On_render() const
{
    player_dash->On_render();
}

void
PlayerStatesDash::On_update(float delta_time)
{
    if(!player.is_on_ground) // 如果不在地面
    {
        player.Switch_to_state(PLAYER_STATE_LEVIATE);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 如果尝试跳跃，切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_PRESS_DASH) &&
            player.is_try_move_x_on_one_dir() &&
            (player.movement_velocity.vx > player.dash_min_speed || player.movement_velocity.vx < -player.dash_min_speed)

    )
    {
        // 1.只有在冲刺键按下
        // 2.并且只按下一个水平移动键
        // 3.并且当前速度大于最小冲刺速度
        // 时，才能冲刺
        player.movement_acceleration += (player.get_facing_vector() * player.dash_acceleration);
    }
    else // 否则退出冲刺状态
    {
        if(player.movement_velocity.vx) // 如果速度不为0
        {
            if(player.is_try_walk())
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
    }
}

void
PlayerStatesDash::On_update_after(float delta_time)
{
    player_dash->On_update(delta_time);
    player_dash->vx = player.movement_position.vx - player_dash->Get_ph_w() / 2;
    player_dash->vy = player.movement_position.vy - player_dash->Get_ph_h();
}

void
PlayerStatesDash::On_exit()
{
    player.is_Lock_facing_dir = false;
}
