#include "player.h"

Player::Player()
    : Object(10.0f, 1.0f, COLOR_RED)
{
}

Player::Player(const Vector2& position, Color color)
    : Object(position, VECTOR2_ZERO, 1.0f, color)
{
}
