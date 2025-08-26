
// header.h

#pragma once

#include "imgui.h"


struct debug_info
{
    const char* name = nullptr;
    int         line = 0;
    const char* file = nullptr;
    ImVec2      rect_pos;
};


void DebugWindow(const debug_info& info);

void 播放音乐(const char* label);
void 渲染图片(const char* label);

void 画背景(const debug_info& info);
void 画前景(const debug_info& info);

void 纹理测试(const debug_info& info);
