
// effect_land.cpp

#include "player_effects.h"

#include "resources_name.h"
#include "resources_pool.h"

static ResourcesPool& resources_pool = ResourcesPool::Instance();

PlayerLandEffect::PlayerLandEffect(const Vector2& position, float center_hight)
    : land_effect_animation(AnimationInstance(*resources_pool.Get_animation(Ani_Player_Action_Effect_Land)))
{
    // 修改动画参数
    land_effect_animation.Set_on_finished([&]() { is_finished = true; });
    land_effect_animation.Set_on_corrective([center_hight](float& x, float& y, const float& w, const float& h) {
        x -= w / 2;
        y -= (h - center_hight);
    });
    land_effect_animation.vx = position.vx;
    land_effect_animation.vy = position.vy;
}

PlayerLandEffect::~PlayerLandEffect()
{
}

void
PlayerLandEffect::On_render() const
{
    // 渲染动画
    land_effect_animation.On_render();
}

void
PlayerLandEffect::On_update(float delta_time)
{
    // 更新动画
    land_effect_animation.On_update(delta_time);
}
