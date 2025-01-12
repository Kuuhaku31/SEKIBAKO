
// player_effect.cpp

#include "player_effect.h"

#include "animation_master.h"
#include "resources_name.h"

static AnimationMaster& animation_master = AnimationMaster::Instance();

AnimationInstance*
CreatPlayerAttackEffectUp()
{
    return animation_master.Create_effect(Ani_Player_Attack_Effect_U);
}

AnimationInstance*
CreatPlayerAttackEffectDown()
{
    return animation_master.Create_effect(Ani_Player_Attack_Effect_D);
}

AnimationInstance*
CreatPlayerAttackEffectLeft()
{
    return animation_master.Create_effect(Ani_Player_Attack_Effect_L);
}

AnimationInstance*
CreatPlayerAttackEffectRight()
{
    return animation_master.Create_effect(Ani_Player_Attack_Effect_R);
}

AnimationInstance*
CreatPlayerJumpEffect()
{
    return animation_master.Create_effect(Ani_Player_Action_Effect_Jump);
}

AnimationInstance*
CreatPlayerLandEffect()
{
    return animation_master.Create_effect(Ani_Player_Action_Effect_Land);
}
