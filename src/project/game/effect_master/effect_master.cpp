
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
    Clear_effects();
}

void
EffectMaster::Register_effect(Effect* effect)
{
    effect_list.push_back(effect);
}

void
EffectMaster::Clear_effects()
{
    for(auto& effect : effect_list) delete effect;
    effect_list.clear();
}

void
EffectMaster::On_update(float delta_time)
{
    // 遍历特效列表
    for(auto it = effect_list.begin(); it != effect_list.end();)
    {
        // 如果特效已经结束
        if((*it)->Is_finished()) // 删除特效
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
