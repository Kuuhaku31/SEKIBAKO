
// effect_master.cpp

#include "effect_master.h"

#include <algorithm>

EffectMaster* EffectMaster::instance = nullptr;
EffectMaster&
EffectMaster::Instance()
{
    if(instance) return *instance;
    instance = new EffectMaster();
    return *instance;
}

Animation*
EffectMaster::Create_effect(const AnimationInfo& effect_info)
{
    Animation* new_animation = new Animation(effect_info);
    effect_list.push_back(new_animation);
    return new_animation;
}

void
EffectMaster::On_update(float delta_time)
{
    // 遍历特效列表
    for(auto it = effect_list.begin(); it != effect_list.end();)
    {
        // 如果特效已经触发过一次
        if((*it)->Get_is_finished()) // 删除特效
        {
            delete *it, it = effect_list.erase(it);
        }
        else // 更新特效
        {
            (*it)->On_update(delta_time), ++it;
        }
    }
}

void
EffectMaster::On_render() const
{
    // 遍历特效列表
    for(auto& effect : effect_list) effect->On_render();
}
