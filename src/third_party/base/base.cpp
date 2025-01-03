
// base.cpp

#include "base.h"

uint32_t
Color::to_int32()
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

bool
operator==(const Point& a, const Point& b)
{
    return a.px == b.px && a.py == b.py;
}

bool
operator!=(const Point& a, const Point& b)
{
    return a.px != b.px || a.py != b.py;
}

bool
operator<(const Point& a, const Point& b)
{
    return a.px < b.px && a.py < b.py;
}

bool
operator>(const Point& a, const Point& b)
{
    return a.px > b.px && a.py > b.py;
}

bool
operator<=(const Point& a, const Point& b)
{
    return a.px <= b.px && a.py <= b.py;
}

bool
operator>=(const Point& a, const Point& b)
{
    return a.px >= b.px && a.py >= b.py;
}

Point
operator+(const Point& a, const Point& b)
{
    return { a.px + b.px, a.py + b.py };
}

Point
operator-(const Point& a, const Point& b)
{
    return { a.px - b.px, a.py - b.py };
}
