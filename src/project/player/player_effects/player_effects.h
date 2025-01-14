
// player_effects.h

#pragma once

#include "animation.h"
#include "effect_master.h"


class PlayerJumpEffect : public Effect
{
public:
    PlayerJumpEffect(const Vector2& position, float center_hight = 0.0f);
    ~PlayerJumpEffect();

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    AnimationInstance jump_effect_animation;
};

class PlayerLandEffect : public Effect
{
public:
    PlayerLandEffect(const Vector2& position);
    ~PlayerLandEffect();

public:
    void On_render() const override;
    void On_update(float delta_time) override;

public:
    AnimationInstance land_effect_animation;
};

class PlayerAttackEffect : public Effect
{
public:
    PlayerAttackEffect(const Vector2& dst_pos, uint8_t attack_dir);
    ~PlayerAttackEffect();

public:
    void On_render() const override;
    void On_update(float delta_time) override;
    void On_update_after(float delta_time) override;

public:
    AnimationInstance attack_effect_animation;

public:
    const Vector2& player_position; // 跟踪角色位置
};
