
// animation_master.h

#pragma once

#include "animation.h"

#include <string>

typedef std::vector<AnimationInstance*> EffectList;

// 特效管理器（单例）
class AnimationMaster
{
public:
    static AnimationMaster& Instance();

public:
    AnimationInstance* Create_effect(const Animation* animtion);  // 创建特效
    AnimationInstance* Create_animtion(const std::string& label); // 创建动画特效

    void On_update(float delta_time); // 更新特效
    void On_render() const;           // 渲染特效

private:
    EffectList effect_list; // 特效列表
    EffectList animation_list;

private:
    AnimationMaster() = default;
    ~AnimationMaster();
    AnimationMaster(const AnimationMaster&)            = delete;
    AnimationMaster& operator=(const AnimationMaster&) = delete;

    static AnimationMaster* instance;
};
