
// animation.cpp

#include "animation.h"

Animation::Animation(const AnimationInfo& info)
{
    // 必须设置的参数
    texture   = info.texture;
    texs_size = info.texs_size;

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

    frame_w     = tex_w / info.num_x;
    frame_h     = tex_h / info.num_y;
    frame_count = info.frame_count;

    frame_src_list = new Point[frame_count];
    for(uint16_t i = 0; i < frame_count; i++)
    {
        uint16_t idx = info.frame_idx_list[i];

        Point& frame_src = frame_src_list[i];

        frame_src.px = (idx % info.num_x) * frame_w;
        frame_src.py = (idx / info.num_x) * frame_h;
    }

    // 可选参数
    if(info.frame_interval > 0) frame_interval = info.frame_interval;
    angle = info.angle;
    if(info.texs_size > 0) texs_size = info.texs_size;
    is_loop = info.is_loop;
}

Animation::~Animation()
{
    delete[] frame_src_list;
}


AnimationInstance::AnimationInstance(const Animation& animation)
    : animation(animation)
{
    // 帧切换计时器
    frame_timer.Set_wait_time(animation.frame_interval);
    Callback f = [&]() {
        frame_current++;
        if(frame_current >= animation.frame_count) // 如果超出帧数
        {
            if(animation.is_loop) // 如果是循环播放
            {
                frame_current = 0;
            }
            else // 如果不是循环播放
            {
                frame_timer.is_paused = true; // 暂停计时器
                is_finished           = true; // 动画结束
                frame_current         = animation.frame_count - 1;
                if(on_finished) on_finished();
            }
        }
    };
    frame_timer.Set_on_timeout(f);   // 设置回调函数
    frame_timer.is_one_shot = false; // 默认设置为循环播放

    angle     = animation.angle;
    texs_size = animation.texs_size;
    ph_w      = animation.frame_w / texs_size;
    ph_h      = animation.frame_h / texs_size;
}

void
AnimationInstance::On_render() const
{
    static const Painter& painter = Painter::Instance();

    static IRect src_rect;
    static FRect dst_rect;

    src_rect.x = animation.frame_src_list[frame_current].px;
    src_rect.y = animation.frame_src_list[frame_current].py;
    src_rect.w = animation.frame_w;
    src_rect.h = animation.frame_h;

    dst_rect.x = vx;
    dst_rect.y = vy;
    dst_rect.w = animation.frame_w / texs_size; // w 表示纹理单位长度 = 纹理像素长度 / texs_size
    dst_rect.h = animation.frame_h / texs_size; // h 表示纹理单位长度 = 纹理像素长度 / texs_size

    painter.DrawTexture(animation.texture, src_rect, dst_rect, angle);
}

void
AnimationInstance::Animation_reset()
{
    is_finished   = false;
    frame_current = 0;
    frame_timer.Restart();
}
