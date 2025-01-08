
// animation.h

#pragma once

#include "base.h"
#include "imgui_setup.h"

struct AnimationInfo
{
    // 必须设置的参数
    Texture*        texture        = nullptr; // 纹理
    const uint16_t* frame_idx_list = nullptr; // 帧索引列表
    uint16_t        frame_count    = 0;       // 帧数
    uint16_t        num_x          = 0;       // 纹理横向切成多少帧
    uint16_t        num_y          = 0;       // 纹理纵向切成多少帧
    float           texs_size      = 0.0f;    // 对于这个纹理，一个单位长度等于 texs_size 个像素

    // 可选参数
    float    interval    = 0.0f;    // 帧间隔
    bool     is_loop     = true;    // 是否循环播放
    Callback on_finished = nullptr; // 动画结束回调
};

// 动画类，支持多种方式添加帧，支持循环播放，支持回调，支持设置渲染中心
class Animation : public Timer, public FRect
{
public:
    Animation(const AnimationInfo& info);
    ~Animation();

public:
    void On_render() const; // 渲染

public:
    double angle   = 0;    // 渲染角度
    bool   is_loop = true; // 是否循环播放

    void Reset();                     // 重置
    void Set_on_finished(Callback f); // 设置动画结束回调

private:
    Texture* texture       = nullptr; // 纹理
    IRect*   rect_src_list = nullptr; // 源矩形列表，用于裁剪纹理
    uint16_t frame_current = 0;       // 当前帧
    uint16_t frame_count   = 0;       // 帧数
    uint16_t frame_w       = 0;       // 帧宽
    uint16_t frame_h       = 0;       // 帧高

    float texs_size = 0.0f; // 对于这个纹理，一个单位长度等于 texs_size 个像素

    Callback on_finished; // 动画结束回调
};
