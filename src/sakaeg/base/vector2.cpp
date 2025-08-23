
// vector2.cpp

#include "base.h"

#include <cmath>

Vector2::Vector2(float x, float y)
    : x(x)
    , y(y)
{
}

Vector2
Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

void
Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

Vector2
Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

void
Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

float
Vector2::operator*(const Vector2& v) const
{
    return x * v.x + y * v.y;
}

Vector2
Vector2::operator*(float f) const
{
    return Vector2(x * f, y * f);
}

void
Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
}

Vector2
Vector2::operator/(float f) const
{
    return Vector2(x / f, y / f);
}

void
Vector2::operator/=(float f)
{
    x /= f;
    y /= f;
}

bool
Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

bool
Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}

bool
Vector2::operator>(const Vector2& v) const
{
    return module() > v.module();
}

bool
Vector2::operator<(const Vector2& v) const
{
    return module() < v.module();
}

Vector2::
operator float*()
{
    return &x;
}

Vector2::
operator Point() const
{
    return { (int)std::floor(x), (int)std::floor(y) };
}

void
Vector2::rotate(float angle)
{
    x = x * cos(angle) - y * sin(angle);
    y = x * sin(angle) + y * cos(angle);
}

void
Vector2::to_unit()
{
    float m = module();

    if(m != 0)
    {
        x /= m;
        y /= m;
    }
}

void
Vector2::to_zero()
{
    x = 0;
    y = 0;
}

float
Vector2::module() const
{
    return sqrt(x * x + y * y);
}

bool
Vector2::approx_zero() const
{
    return module() < 1e-4;
}
