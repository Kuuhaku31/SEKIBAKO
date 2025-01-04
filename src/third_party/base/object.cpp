
// object.cpp

#include "base.h"

#include "imgui_setup.h"

Object::Object(const Vector2& pos, const Vector2& vel, float mass, const Color& color)
    : movement_position(pos)
    , movement_velocity(vel)
    , movement_mass(mass)
    , object_color(color)
{
}

Object::Object(float radius, float mass, const Color& color)
    : object_radius(radius)
    , movement_mass(mass)
    , object_color(color)
{
}

void
Object::On_update(float delta_time)
{
    // 更新速度
    movement_velocity += (movement_acceleration * delta_time);
    if(movement_velocity.module() < 1.0f) movement_velocity.to_zero();

    // 更新位置
    movement_position += (movement_velocity * delta_time);

    // 清空加速度
    movement_acceleration.to_zero();
}

void
Object::On_render() const
{
    static const Painter& painter = Painter::Instance();

    painter.DrawCircle(
        movement_position.vx,
        movement_position.vy,
        object_radius,
        object_color,
        true

    );
}

void
Object::Force(const Vector2& force)
{
    if(movement_mass <= 0) return;
    movement_acceleration += (force / movement_mass);
}

void
Object::Force_resistance(float friction, float air_resistance)
{
    static float   v_mod = 0.0f;
    static Vector2 dir;

    v_mod = movement_velocity.module();
    if(v_mod > 0)
    {
        // 根据阻力参数改变运动状态
        dir = movement_velocity;
        dir.to_unit();
        Force(dir * -(friction + v_mod * v_mod * air_resistance));
    }
}

void
Object::Move(const Vector2& move)
{
    movement_position += move;
}

const Vector2&
Object::Get_position() const
{
    return movement_position;
}

const Vector2&
Object::Get_velocity() const
{
    return movement_velocity;
}

const Vector2&
Object::Get_acceleration() const
{
    return movement_acceleration;
}

float
Object::Get_mass() const
{
    return movement_mass;
}

float
Object::Get_radius() const
{
    return object_radius;
}

const Color&
Object::Get_color() const
{
    return object_color;
}

void
Object::Set_position(const Vector2& position)
{
    movement_position = position;
}

void
Object::Set_velocity(const Vector2& velocity)
{
    movement_velocity = velocity;
}

void
Object::Set_acceleration(const Vector2& acceleration)
{
    movement_acceleration = acceleration;
}

void
Object::Set_mass(float mass)
{
    movement_mass = mass;
}

void
Object::Set_radius(float radius)
{
    object_radius = radius;
}

void
Object::Set_color(const Color& color)
{
    object_color = color;
}
