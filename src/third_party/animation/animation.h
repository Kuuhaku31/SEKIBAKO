
// animation.h

#pragma once

#include "base.h"
#include "imgui_setup.h"

struct AnimationInfo
{
    // 必须设置的参数
    Texture*  texture;        // 纹理
    uint16_t  num_x;          // 纹理横向切成多少帧
    uint16_t  num_y;          // 纹理纵向切成多少帧
    uint16_t* frame_idx_list; // 帧索引列表
    uint16_t  frame_count;    // 帧数

    // 可选参数
    float frame_interval = 0.0f; // 帧间隔
    float angle          = 0.0f; // 渲染角度
    float texs_size      = 0.0f; // 渲染大小，对于这个纹理，一个单位长度等于 texs_size 个像素
    bool  is_loop        = true; // 是否循环播放
};

// 动画类
class Animation
{
    friend class AnimationInstance;

public:
    Animation(const AnimationInfo& info);
    ~Animation();

private:
    Texture* texture;        // 纹理
    Point*   frame_src_list; // 帧源列表
    uint16_t frame_w;        // 帧宽
    uint16_t frame_h;        // 帧高
    uint16_t frame_count;    // 帧数

    float frame_interval = 0.1f; // 帧间隔
    float angle          = 0.0f; // 渲染角度
    float texs_size      = 1.0f; // 渲染大小，对于这个纹理，一个单位长度等于 texs_size 个像素
    bool  is_loop        = true; // 是否循环播放
};

// 动画实例
class AnimationInstance : public Vector2
{
public:
    AnimationInstance(const Animation& animation);
    ~AnimationInstance() = default;

public:
    void On_render() const;                                                 // 渲染
    void On_update(float delta_time) { frame_timer.On_update(delta_time); } // 更新

public:
    void Animation_reset(); // 重置

    void Set_on_finished(Callback f) { on_finished = f; } // 设置结束回调

    float Get_ph_w() const { return ph_w; } // 物理宽
    float Get_ph_h() const { return ph_h; } // 物理高

    const bool& Get_is_finished() const { return is_finished; } // 动画是否结束

private:
    const Animation& animation;

    Timer    frame_timer;           // 帧计时器
    uint16_t frame_current = 0;     // 当前帧
    bool     is_finished   = false; // 动画是否结束

    float angle;     // 渲染角度
    float texs_size; // 渲染大小，对于这个纹理，一个单位长度等于 texs_size 个像素
    float ph_w;      // 物理宽
    float ph_h;      // 物理高

    Callback on_finished; // 动画结束回调
};
