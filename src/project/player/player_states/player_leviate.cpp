
// player_leviate.cpp

#include "player.h"
#include "player_states.h"

#include "animation_master.h"
#include "player_effect.h"

PlayerStatesLeviate::PlayerStatesLeviate(Player& player)
    : StateNode(PLAYER_STATE_LEVIATE)
    , player(player)
{
    player_leviate = AnimationMaster::Instance().Create_animtion("Ani-SEKIBAKO-Leviate-R");
}

void
PlayerStatesLeviate::On_enter()
{
    player.object_color = PLAYER_LEVIATE_COLOR;

    player.is_on_ground   = false;
    player.enable_gravity = true;

    player.is_use_friction       = false;
    player.is_use_air_resistance = true;
}

void
PlayerStatesLeviate::On_render() const
{
    player_leviate->On_render();
}

void
PlayerStatesLeviate::On_update(float delta_time)
{
    if(player.movement_position.vy >= player.floor_correct_y)
    {
        // 如果接触到地面，必须退出 leviate 状态
        make_land_effect();

        if(player.movement_velocity.vx) // 如果速度不为0
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
            // 如果速度为0，切换到 idle 状态
            player.Switch_to_state(PLAYER_STATE_IDLE);
        }
    }
    else if(CONTROLER_GET(player.player_controler, PLAYER_CONTROL_CLICK_JUMP) && player.can_jump_count > 0)
    {
        // 1. 如果尝试跳跃
        // 2. 且可以跳跃
        // 切换到 jump 状态
        player.Switch_to_state(PLAYER_STATE_JUMP);
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
PlayerStatesLeviate::On_update_after(float delta_time)
{
    player_leviate->On_update(delta_time);
    player_leviate->vx = player.movement_position.vx - player_leviate->Get_ph_w() / 2;
    player_leviate->vy = player.movement_position.vy - player_leviate->Get_ph_h();
}

void
PlayerStatesLeviate::On_exit()
{
}

void
PlayerStatesLeviate::make_land_effect()
{
    AnimationInstance* land_effect = CreatPlayerLandEffect();

    land_effect->vx = player.movement_position.vx - land_effect->Get_ph_w() / 2;
    land_effect->vy = player.floor_correct_y - land_effect->Get_ph_h() + player.object_radius;
}
