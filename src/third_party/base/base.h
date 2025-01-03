
// base.h

#pragma once

#include <functional>
#include <stdint.h>


class IRect
{
public:
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = 0;
    int32_t h = 0;
};

class FRect
{
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
};

struct Point
{
    int32_t px = 0;
    int32_t py = 0;
};


class Vector2
{
public:
    float vx = 0;
    float vy = 0;

public:
    Vector2() = default;
    Vector2(float x, float y);
    ~Vector2() = default;

public:
    Vector2 operator+(const Vector2& v) const;
    void    operator+=(const Vector2& v);
    Vector2 operator-(const Vector2& v) const;
    void    operator-=(const Vector2& v);
    float   operator*(const Vector2& v) const;
    Vector2 operator*(float f) const;
    void    operator*=(float f);
    Vector2 operator/(float f) const;
    void    operator/=(float f);

    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;
    bool operator>(const Vector2& v) const;
    bool operator<(const Vector2& v) const;

    operator float*(); // 定义强制转换运算符，将 Vector2 转换为 float*
    operator Point() const;

    void rotate(float angle);
    void to_unit();
    void to_zero();

    float module() const;

    bool approx_zero() const;
};


class Timer
{
public:
    Timer()  = default;
    ~Timer() = default;

public:
    void restart();
    void set_wait_time(float t);
    void set_one_shot(bool flag);
    void set_on_timeout(std::function<void()> f);
    void pause();  // 暂停计时器
    void resume(); // 恢复计时器
    void on_update(float dt);
    void shot(); // 立即触发

private:
    float pass_time = 0;     // 计时器已经过去的时间
    float wait_time = 0;     // 计时器等待的时间
    bool  paused    = false; // 计时器是否暂停
    bool  shotted   = false; // 计时器是否已经触发过一次
    bool  one_shot  = false; // 计时器是否为一次性触发

    // 超时回调函数
    std::function<void()> on_timeout = nullptr;
};


// 最基础的图形类，用四个字节的数组表示一个矩阵，每个字节表示一个像素点的颜色
class Shape
{
public:
    Shape() = default;
    ~Shape(); // **记得释放内存**

public:
    // 重新设置形状
    void Shape_reset(int = 0, int = 0, int = 0);
    void Shape_reset(const int*, int, int);

    // 合并两个形状
    void Shape_merge(Shape*, std::function<void(int&, int&)> f, int dx = 0, int dy = 0);

    // 获取形状的宽高、缓冲区大小
    int Shape_wide() const;
    int Shape_high() const;
    int Shape_long() const;

    int* Shape_buffer() const;

    // 获取形状某个点的值
    int Shape_in(int) const;
    int Shape_in(int, int) const;

    // 绘制圆形
    void Shape_draw_point(int, int, int = 0);
    void Shape_draw_line(int, int, int, int, int = 0);
    void Shape_draw_rectangle(int, int, int, int, int = 0);
    void Shape_draw_circle(int, int, int, int = 0);

    // 复制形状
    void Shape_copy(Shape*);
    // 设置所有单位的值
    void Shape_clear(int = 0);

protected:
    int* shape_buffer = nullptr;
    int  shape_wide   = 0;
    int  shape_high   = 0;
    int  shape_long   = 0;
};


// Object
class Object
{
public:
    Object(const Vector2& pos = { 0, 0 }, const Vector2& vel = { 0, 0 }, float mass = 0, int color = 0);
    ~Object() = default;

public:
    virtual void On_update(float delta_time);
    virtual void On_render() const;

    void Force(const Vector2& force);

    const Vector2& Get_position() const;
    const Vector2& Get_velocity() const;
    const Vector2& Get_acceleration() const;
    float          Get_mass() const;
    float          Get_radius() const;
    int            Get_color() const;

    void Set_position(const Vector2& position);
    void Set_velocity(const Vector2& velocity);
    void Set_acceleration(const Vector2& acceleration);
    void Set_mass(float mass);
    void Set_resistance_friction(float resistance_friction);
    void Set_resistance_air(float resistance_air);
    void Set_radius(float radius);
    void Set_color(int color);

protected:
    Vector2 movement_position;                // 位置
    Vector2 movement_velocity;                // 速度
    Vector2 movement_acceleration;            // 加速度
    float   movement_mass                = 0; // 质量（为0时视为质量无穷大）
    float   movement_resistance_friction = 0; // 摩擦力
    float   movement_resistance_air      = 0; // 空气阻力
    float   object_radius                = 0; // 半径
    int     object_color                 = 0; // 颜色
};


// View
class View
{
public:
    void Set_view_center_position(const Vector2& pos);
    void Set_view_size(const Vector2& size);
    void Set_unit_size(float size);

    const Vector2& Get_view_center_position() const;
    const Vector2& Get_view_size() const;
    const Vector2& Get_view_size_half() const;
    const Vector2& Get_view_left_top_position() const;
    const Vector2& Get_view_right_bottom_position() const;

    const float& Get_unit_size() const;

private:
    Vector2 view_center_position;       // 视野中心位置
    Vector2 view_size;                  // 视野宽高
    Vector2 view_size_half;             // 视野宽高的一半
    Vector2 view_left_top_position;     // 视野左上角位置
    Vector2 view_right_bottom_position; // 视野右下角位置

    float unit_size = 1.0f; // 一个单位长度的大小（像素）
};


template<typename Tem>
void
swap(Tem& a, Tem& b)
{
    static Tem c;

    c = a;
    a = b;
    b = c;
}

bool
operator==(const Point& a, const Point& b);

bool
operator!=(const Point& a, const Point& b);

bool
operator<(const Point& a, const Point& b);

bool
operator>(const Point& a, const Point& b);

bool
operator<=(const Point& a, const Point& b);

bool
operator>=(const Point& a, const Point& b);

Point
operator+(const Point& a, const Point& b);

Point
operator-(const Point& a, const Point& b);
