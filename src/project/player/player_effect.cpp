
// player_effect.cpp

#include "player_effect.h"

#include "effect_master.h"
#include "resources_name.h"
#include "resources_pool.h"

static ResourcesPool& resources_pool = ResourcesPool::Instance();
static EffectMaster&  effect_master  = EffectMaster::Instance();

AnimationInstance*
CreatPlayerAttackEffectUp()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_U));
}

AnimationInstance*
CreatPlayerAttackEffectDown()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_D));
}

AnimationInstance*
CreatPlayerAttackEffectLeft()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_L));
}

AnimationInstance*
CreatPlayerAttackEffectRight()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Attack_Effect_R));
}

AnimationInstance*
CreatPlayerJumpEffect()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Action_Effect_Jump));
}

AnimationInstance*
CreatPlayerLandEffect()
{
    return effect_master.Create_effect(resources_pool.Get_animation(Ani_Player_Action_Effect_Land));
}
