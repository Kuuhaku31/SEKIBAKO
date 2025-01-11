
// effect_master.cpp

#include "effect_master.h"

#include "resources_pool.h"

#include <algorithm>

static ResourcesPool& resources_pool = ResourcesPool::Instance();

EffectMaster* EffectMaster::instance = nullptr;
EffectMaster&
EffectMaster::Instance()
{
    if(instance) return *instance;
    instance = new EffectMaster();
    return *instance;
}

EffectMaster::~EffectMaster()
{
    for(auto& effect : effect_list) delete effect;
    for(auto& effect : animation_list) delete effect;
    effect_list.clear();
    animation_list.clear();
}

AnimationInstance*
EffectMaster::Create_effect(const Animation* animtion)
{
    if(!animtion) return nullptr;
    AnimationInstance* new_effect = new AnimationInstance(*animtion);
    effect_list.push_back(new_effect);
    return new_effect;
}

AnimationInstance*
EffectMaster::Create_animtion(const std::string& label)
{
    static Animation* ani = nullptr;

    if(!(ani = resources_pool.Get_animation(label))) return nullptr;

    AnimationInstance* new_effect = new AnimationInstance(*ani);
    animation_list.push_back(new_effect);
    return new_effect;
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
            delete *it;
            it = effect_list.erase(it);
        }
        else // 更新特效
        {
            (*it)->On_update(delta_time);
            ++it;
        }
    }
}

void
EffectMaster::On_render() const
{
    // 遍历特效列表
    for(auto& effect : effect_list) effect->On_render();
}
