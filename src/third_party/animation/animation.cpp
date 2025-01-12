
// animation.cpp

#include "animation.h"

#include "imgui_setup.h"

// 动画模板
AnimationTemplate::AnimationTemplate(const AnimationInformation& info)
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
    is_loop             = info.is_loop;
    position_correction = info.position_correction;
}

AnimationTemplate::~AnimationTemplate()
{
    delete[] frame_src_list;
}

// 动画实例
AnimationInstance::AnimationInstance(const AnimationTemplate& animation, Callback animation_finished_callback)
    : animation(animation)
{
    // 帧切换计时器
    frame_timer.Set_wait_time(animation.frame_interval);
    Callback timer_callback = [&]() {
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
    frame_timer.Set_on_timeout(timer_callback); // 设置回调函数
    frame_timer.is_one_shot = false;            // 默认设置为循环播放

    angle               = animation.angle;
    position_correction = animation.position_correction;

    texs_size = animation.texs_size;
    ph_w      = animation.frame_w / texs_size; // w 表示纹理单位长度 = 纹理像素长度 / texs_size
    ph_h      = animation.frame_h / texs_size; // h 表示纹理单位长度 = 纹理像素长度 / texs_size

    on_finished = animation_finished_callback;
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

    dst_rect.x = vx + position_correction.vx;
    dst_rect.y = vy + position_correction.vy;
    dst_rect.w = ph_w;
    dst_rect.h = ph_h;

    painter.DrawTexture(animation.texture, src_rect, dst_rect, angle);
}

void
AnimationInstance::Restart()
{
    is_finished   = false;
    frame_current = 0;
    frame_timer.Restart();
}

void
AnimationInstance::Set_play_time(float t)
{
    if(t <= 0) return;

    frame_timer.Set_wait_time(t / animation.frame_count);
}

void
AnimationInstance::Set_on_finished(Callback f)
{
    on_finished = f;
} // 设置结束回调

void
AnimationInstance::Set_frame_interval(float interval)
{
    frame_timer.Set_wait_time(interval);
}

void
AnimationInstance::Set_frame_interval_add(float interval)
{
    frame_timer.Set_wait_time_add(interval);
}

void
AnimationInstance::Set_frame_interval_mul(float interval)
{
    frame_timer.Set_wait_time_mul(interval);
}

void
AnimationInstance::Set_size(float size)
{
    if(size > 0)
    {
        texs_size = size;
        update_ph_vy();
    }
}

void
AnimationInstance::Set_size_add(float size)
{
    if(texs_size + size > 0)
    {
        texs_size += size;
        update_ph_vy();
    }
}

void
AnimationInstance::Set_size_mul(float size)
{
    if(size > 0)
    {
        texs_size *= size;
        update_ph_vy();
    }
}
