
// effect_master.h

#pragma once

#include "animation.h"

#include <string>

class Effect
{
public:
    Effect()  = default;
    ~Effect() = default;

public:
    virtual void On_render() const {};           // 渲染特效
    virtual void On_update(float delta_time) {}; // 更新特效
    virtual void On_update_after(float delta_time) {};

public:
    const bool& Is_finished() const { return is_finished; }

protected:
    bool is_finished = false; // 特效是否完成
};

typedef std::vector<Effect*> EffectList;

// 特效管理器（单例）
class EffectMaster
{
public:
    static EffectMaster& Instance();

public:
    void On_update(float delta_time); // 更新特效
    void On_render() const;           // 渲染特效

    void Register_effect(Effect* effect); // 创建特效
    void Clear_effects();

private:
    EffectList effect_list; // 特效列表

private:
    EffectMaster() = default;
    ~EffectMaster();
    EffectMaster(const EffectMaster&)            = delete;
    EffectMaster& operator=(const EffectMaster&) = delete;

    static EffectMaster* instance;
};
