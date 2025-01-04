
// color.h

#pragma once

#include <stdint.h>

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color&
    operator=(const Color& color)
    {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;

        return *this;
    }
};

#define COLOR_BLACK Color{ 0x00, 0x00, 0x00, 0xFF }       // 黑色
#define COLOR_WHITE Color{ 0xFF, 0xFF, 0xFF, 0xFF }       // 白色
#define COLOR_RED Color{ 0xFF, 0x00, 0x00, 0xFF }         // 红色
#define COLOR_GREEN Color{ 0x00, 0xFF, 0x00, 0xFF }       // 绿色
#define COLOR_BLUE Color{ 0x00, 0x00, 0xFF, 0xFF }        // 蓝色
#define COLOR_YELLOW Color{ 0xFF, 0xFF, 0x00, 0xFF }      // 黄色
#define COLOR_CYAN Color{ 0x00, 0xFF, 0xFF, 0xFF }        // 青色
#define COLOR_PURPLE Color{ 0xFF, 0x00, 0xFF, 0xFF }      // 紫色
#define COLOR_ORANGE Color{ 0xFF, 0xA5, 0x00, 0xFF }      // 橙色
#define COLOR_GRAY Color{ 0x80, 0x80, 0x80, 0xFF }        // 灰色
#define COLOR_DARK_GRAY Color{ 0x40, 0x40, 0x40, 0xFF }   // 深灰色
#define COLOR_LIGHT_GRAY Color{ 0xC0, 0xC0, 0xC0, 0xFF }  // 浅灰色
#define COLOR_TRANSPARENT Color{ 0x00, 0x00, 0x00, 0x00 } // 透明色
