#include "effect_master.h"

EffectMaster* EffectMaster::instance = nullptr;
EffectMaster&
EffectMaster::Instance()
{
    if(instance) return *instance;
    instance = new EffectMaster();
    return *instance;
}

void
EffectMaster::Create_effect(const AnimationInfo& effect_info)
{
    effect_list.push_back(new Animation(effect_info));
}

void
EffectMaster::On_update(float delta_time)
{
    for(auto& effect : effect_list)
    {
        // if()

        effect->On_update(delta_time);
    }
}
