
// animation.cpp

#include "animation.h"

Animation::Animation(const AnimationInfo& info)
{
    texture   = info.texture;
    texs_size = info.texs_size;

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

    frame_w = tex_w / info.num_x;
    frame_h = tex_h / info.num_y;

    w = frame_w / texs_size; // w 表示纹理单位长度 = 纹理像素长度 / texs_size
    h = frame_h / texs_size;

    frame_count   = info.frame_count;
    rect_src_list = new IRect[frame_count];
    for(uint16_t i = 0; i < frame_count; i++)
    {
        uint16_t idx = info.frame_idx_list[i];

        IRect& rect_src = rect_src_list[i];

        rect_src.x = (idx % info.num_x) * frame_w;
        rect_src.y = (idx / info.num_x) * frame_h;
        rect_src.w = frame_w;
        rect_src.h = frame_h;
    }

    // 帧切换计时器
    Callback f = [&]() {
        frame_current++;
        if(frame_current >= frame_count) // 如果超出帧数
        {
            if(is_loop) // 如果是循环播放
            {
                frame_current = 0;
            }
            else // 如果不是循环播放
            {
                is_paused     = true; // 暂停计时器
                is_finished   = true; // 动画结束
                frame_current = frame_count - 1;
                if(on_finished) on_finished();
            }
        }
    };
    Set_on_timeout(f);   // 设置回调函数
    is_one_shot = false; // 默认设置为循环播放

    // 其他参数
    Set_wait_time(info.interval);
    is_loop     = info.is_loop;
    on_finished = info.on_finished;
}

Animation::~Animation()
{
    delete[] rect_src_list;
}

// 重置
void
Animation::Animation_reset()
{
    is_finished   = false;
    frame_current = 0;
    Restart();
}

// 设置动画结束回调
void
Animation::Set_on_finished(Callback f)
{
    on_finished = f;
}

// 渲染
void
Animation::On_render() const
{
    static Painter& painter = Painter::Instance();

    painter.DrawTexture(texture, rect_src_list[frame_current], *this, angle);
}
