
// effect_master.h

#pragma once

#include "animation.h"

typedef std::vector<Animation*> EffectList;

// 特效管理器（单例）
class EffectMaster
{
public:
    static EffectMaster& Instance();

public:
    Animation* Create_effect(const AnimationInfo& effect_info); // 创建特效

    void On_update(float delta_time); // 更新特效
    void On_render() const;           // 渲染特效

private:
    EffectList effect_list; // 特效列表

private:
    EffectMaster()                               = default;
    ~EffectMaster()                              = default;
    EffectMaster(const EffectMaster&)            = delete;
    EffectMaster& operator=(const EffectMaster&) = delete;

    static EffectMaster* instance;
};
