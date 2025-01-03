
// 动画类，支持多种方式添加帧，支持循环播放，支持回调，支持设置渲染中心

#pragma once

#include "base.h"
#include "imgui_setup.h"

class Animation
{
public:
    Animation();
    ~Animation() = default;

public:
    void reset();                                  // 重置
    void set_loop(bool flag);                      // 设置是否循环播放
    void set_interval(double interval);            // 设置帧间隔
    void set_on_finished(std::function<void()> f); // 设置动画结束回调

    // 利用一个纹理添加帧
    void add_frame(Texture* tex, int num_x, int num_y, const std::vector<int>& idx_list);

    void on_update(double dt);                                                  // 更新
    void on_render(Renderer* renderer, const Point& pos_dis, double angle = 0); // 渲染

private:
    Texture* texture = nullptr;

    std::vector<SDL_Rect> rect_src_list; // 源矩形列表，用于裁剪纹理

    uint16_t idx_frame = 0; // 当前帧索引

    int frame_wide = 0;
    int frame_high = 0;

    Timer timer;
    bool  is_loop = true;

    std::function<void()> on_finished;
};
