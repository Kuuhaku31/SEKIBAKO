
// base.cpp

#include "base.h"


bool
operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool
operator!=(const Point& a, const Point& b)
{
    return a.x != b.x || a.y != b.y;
}

bool
operator<(const Point& a, const Point& b)
{
    return a.x < b.x && a.y < b.y;
}

bool
operator>(const Point& a, const Point& b)
{
    return a.x > b.x && a.y > b.y;
}

bool
operator<=(const Point& a, const Point& b)
{
    return a.x <= b.x && a.y <= b.y;
}

bool
operator>=(const Point& a, const Point& b)
{
    return a.x >= b.x && a.y >= b.y;
}

Point
operator+(const Point& a, const Point& b)
{
    return { a.x + b.x, a.y + b.y };
}

Point
operator-(const Point& a, const Point& b)
{
    return { a.x - b.x, a.y - b.y };
}
