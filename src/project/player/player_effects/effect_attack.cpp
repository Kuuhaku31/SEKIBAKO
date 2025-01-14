
// effect_attack.cpp

#include "player_effects.h"

#include "resources_name.h"
#include "resources_pool.h"

static ResourcesPool& resources_pool = ResourcesPool::Instance();

inline std::string
parse_dir(uint8_t dir)
{
    switch(dir)
    {
    case 0: return Ani_Player_Attack_Effect_U;
    case 1: return Ani_Player_Attack_Effect_D;
    case 2: return Ani_Player_Attack_Effect_L;
    default:
    case 3: return Ani_Player_Attack_Effect_R;
    }
}

inline CorrectivePos
parse_corrective_pos(uint8_t dir)
{
    switch(dir)
    {
    case 0: return [](float& x, float& y, const float& w, const float& h) {
        x -= w / 2;
        y -= h;
    };
    case 1: return [](float& x, float& y, const float& w, const float& h) {
        x -= w / 2;
    };
    case 2: return [](float& x, float& y, const float& w, const float& h) {
        x -= w;
        y -= h / 2;
    };
    default:
    case 3: return [](float& x, float& y, const float& w, const float& h) {
        y -= h / 2;
    };
    }
}

PlayerAttackEffect::PlayerAttackEffect(const Vector2& dst_pos, uint8_t attack_dir)
    : player_position(dst_pos)
    , attack_effect_animation(AnimationInstance(*resources_pool.Get_animation(parse_dir(attack_dir))))
{
    // 修改动画参数
    attack_effect_animation.Set_on_finished([&]() { is_finished = true; });
    attack_effect_animation.Set_on_corrective(parse_corrective_pos(attack_dir));
}

PlayerAttackEffect::~PlayerAttackEffect()
{
}

void
PlayerAttackEffect::On_render() const
{
    // 渲染动画
    attack_effect_animation.On_render();
}

void
PlayerAttackEffect::On_update(float delta_time)
{
    // 更新动画
    attack_effect_animation.On_update(delta_time);
}

void
PlayerAttackEffect::On_update_after(float delta_time)
{
    // 跟踪角色位置
    attack_effect_animation.vx = player_position.vx;
    attack_effect_animation.vy = player_position.vy;
}
