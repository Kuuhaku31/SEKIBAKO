
// timer.h

#include "base.h"

// 重启计时器
void
Timer::restart()
{
    pass_time = 0;
    shotted   = false;
    paused    = false;
}

// 设置等待时间
void
Timer::set_wait_time(float t)
{
    wait_time = t;
}

// 设置是否为一次性触发
void
Timer::set_one_shot(bool flag)
{
    one_shot = flag;
}

// 设置超时回调函数
void
Timer::set_on_timeout(std::function<void()> f)
{
    on_timeout = f;
}

// 暂停计时器
void
Timer::pause()
{
    paused = true;
}

// 恢复计时器
void
Timer::resume()
{
    paused = false;
}

// 更新计时器
void
Timer::On_update(float dt)
{
    // 如果计时器暂停，则不更新
    if(paused)
    {
        return;
    }

    // 更新计时器
    pass_time += dt;

    // 如果计时器超时
    if(pass_time >= wait_time)
    {
        // 如果计时器为一次性触发，则不再触发
        bool can_shot = (!one_shot || (one_shot && !shotted));
        shotted       = true;

        // 如果可以触发超时回调函数
        if(can_shot && on_timeout)
        {
            on_timeout();
        }

        // 重置计时器
        pass_time -= wait_time;
    }
}

// 立即触发
void
Timer::shot()
{
    shotted = true;
    on_timeout();
}
