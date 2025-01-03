
// Animation 类

#include "animation.h"

Animation::Animation()
{
    // lambda 表达式
    std::function<void()> f = [&]() {
        idx_frame++;
        if(idx_frame >= rect_src_list.size())
        {
            idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
            if(!is_loop && on_finished)
            {
                on_finished();
            }
        }
    };

    timer.set_one_shot(false); // 默认设置为循环播放
    timer.set_on_timeout(f);   // 设置回调函数
}

// 重置
void
Animation::reset()
{
    timer.restart();
    idx_frame = 0;
}

// 设置是否循环播放
void
Animation::set_loop(bool flag)
{
    is_loop = flag;
}

// 设置帧间隔
void
Animation::set_interval(double interval)
{
    timer.set_wait_time(interval);
}

// 设置动画结束回调
void
Animation::set_on_finished(std::function<void()> f)
{
    on_finished = f;
}

void
Animation::add_frame(Texture* tex, int num_x, int num_y, const std::vector<int>& idx_list)
{
    texture = tex;

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

    frame_wide = tex_w / num_x;
    frame_high = tex_h / num_y;

    rect_src_list.resize(idx_list.size());
    for(int i = 0; i < idx_list.size(); i++)
    {
        int idx = idx_list[i];

        SDL_Rect& rect_src = rect_src_list[i];

        rect_src.x = (idx % num_x) * frame_wide;
        rect_src.y = (idx / num_x) * frame_high;
        rect_src.w = frame_wide;
        rect_src.h = frame_high;
    }
}

// 更新
void
Animation::on_update(double dt)
{
    timer.on_update(dt);
}

// 渲染
void
Animation::on_render(Renderer* renderer, const Point& pos_dis, double angle)
{
    static SDL_Rect rect_dst;

    rect_dst.x = pos_dis.px;
    rect_dst.y = pos_dis.py;
    rect_dst.w = frame_wide;
    rect_dst.h = frame_high;

    // 根据 anchor_mode 设置渲染中心
    SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_FLIP_NONE);
}
