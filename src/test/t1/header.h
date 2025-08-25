
// header.h

#pragma once


struct debug_info
{
    const char* name = nullptr;
    int         line = 0;
    const char* file = nullptr;
};


void DebugWindow(const debug_info& info);
void ShowMusicPlayerUI();

void 渲染图片(const char* label);
