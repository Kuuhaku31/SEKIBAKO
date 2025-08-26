
// draw.cpp


#include "header.h"


#include <stdio.h>

void
纹理测试(const debug_info&)
{
    int width  = 512;
    int height = 512;


    ImGui::Begin("Texture Viewer");
    ImGui::Image((ImTextureID)(intptr_t)1, ImVec2(width, height));
    ImGui::End();
}
