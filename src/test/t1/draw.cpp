
// draw.cpp


#include "header.h"

#include <resources_pool.h>

#include <stdio.h>

void
纹理测试(const debug_info&)
{
    static ImGuiIO& io = ImGui::GetIO();

    static const ResourcesPool&     resources_pool = ResourcesPool::Instance();
    static const AnimationTemplate& test_template  = *resources_pool.Get_animation("Ani-Test");

    static AnimationInstance ani_instance(test_template);

    ani_instance.On_update(io.DeltaTime);

    ImGui::Begin("Texture Viewer");
    // ImGui::Image((ImTextureID)(intptr_t)1, ImVec2(width, height));

    Texture_ptr tex;
    IRect       src_rect;
    FRect       dst_rect;
    ani_instance.GetCurrentFrame(&tex, &src_rect, &dst_rect);

    ImGui::Text("Texture ID: %d", tex->get_id());
    ImGui::Text("src_rect: (%d, %d, %d, %d)", src_rect.x, src_rect.y, src_rect.w, src_rect.h);
    ImGui::Text("dst_rect: (%f, %f, %f, %f)", dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);

    // 坐标归一化
    ImVec2 uv0(src_rect.x / (float)tex->get_size().w, src_rect.y / (float)tex->get_size().h);
    ImVec2 uv1((src_rect.x + src_rect.w) / (float)tex->get_size().w, (src_rect.y + src_rect.h) / (float)tex->get_size().h);

    ImGui::Image((ImTextureID)(intptr_t)tex->get_id(), ImVec2(dst_rect.w, dst_rect.h), uv0, uv1);

    ImGui::End();
}
