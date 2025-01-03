
// object.cpp

#include "base.h"

#include "imgui_setup.h"

Object::Object(const Vector2& pos, const Vector2& vel, float mass, int color)
    : movement_position(pos)
    , movement_velocity(vel)
    , movement_mass(mass)
    , object_color(color)
{
}

void
Object::On_update(float delta_time)
{
    // 更新速度
    movement_velocity += movement_acceleration * delta_time;

    // 如果有阻力参数，则计算阻力
    if(
        movement_mass &&
        movement_resistance_friction &&
        movement_resistance_air &&
        movement_velocity.vx &&
        movement_velocity.vy)
    {
        // 根据阻力参数改变运动状态
        float v_mod  = movement_velocity.module();
        float v_mod_ = v_mod - (movement_resistance_friction + v_mod * v_mod * movement_resistance_air) / movement_mass;

        if(v_mod_ < 0)
        {
            movement_velocity = Vector2{ 0, 0 };
        }
        else
        {
            movement_velocity *= v_mod_ / v_mod;
        }
    }

    // 更新位置
    movement_position += (movement_velocity * delta_time);

    // 清空加速度
    movement_acceleration = { 0.0f, 0.0f };
}

void
Object::On_render() const
{
    static const Painter& painter = Painter::Instance();
    static const float    SIZE    = 0.1;
    static const float    EDGR    = SIZE * sqrt(3);

    static Vector2 vel_unit;
    static Vector2 pos_top;
    static Vector2 pos_left;
    static Vector2 pos_right;

    if(movement_velocity != Vector2{ 0, 0 })
    {
        vel_unit = movement_velocity;
        vel_unit.to_unit();

        pos_top = movement_position + vel_unit * SIZE;

        pos_left  = pos_top - (vel_unit * EDGR);
        pos_right = pos_top - (vel_unit * EDGR);
        pos_left.rotate(-M_PI / 9);
        pos_right.rotate(M_PI / 9);

        painter.DrawTriangle(
            pos_top.vx,
            pos_top.vy,
            pos_left.vx,
            pos_left.vy,
            pos_right.vx,
            pos_right.vy,
            object_color,
            true

        );
    }
    else
    {
        painter.DrawCircle(
            movement_position.vx,
            movement_position.vy,
            SIZE,
            object_color,
            true

        );
    }
}

void
Object::Force(const Vector2& force)
{
    if(movement_mass <= 0) return;
    movement_acceleration += force / movement_mass;
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

int
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
Object::Set_resistance_friction(float resistance_friction)
{
    movement_resistance_friction = resistance_friction;
}

void
Object::Set_resistance_air(float resistance_air)
{
    movement_resistance_air = resistance_air;
}

void
Object::Set_radius(float radius)
{
    object_radius = radius;
}

void
Object::Set_color(int color)
{
    object_color = color;
}
