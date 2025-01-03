
// main.cpp

#include <stdio.h>

#include "imgui_setup.h"
#include "resources_pool.h"

static Painter&       painter        = Painter::Instance();
static ResourcesPool& resources_pool = ResourcesPool::Instance();

bool is_running = true;

Color clear_color = { 0xcc, 0xcc, 0xcc, 0xFF };

std::function<void(const Event&)> f = [](const Event& e) {
    if(e.type == SDL_QUIT) is_running = false;
};

void
ImguiTestWindow(bool* is_open = nullptr)
{
    if(is_open && !*is_open) return;

    ImGui::Begin("Hello, World!", is_open);
    ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SmileySans));

    {

        ImGui::Text("Hello, World!");
        ImGui::Text("中文测试");

        ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SweiAliasLegCJKjp));
        ImGui::Text("日本語テスト");
        ImGui::PopFont();
    }

    {
        ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_ipix));
        // 调整颜色
        {
            static float color_buffer[4] = { 0 };

            color_buffer[0] = clear_color.r / 255.0f;
            color_buffer[1] = clear_color.g / 255.0f;
            color_buffer[2] = clear_color.b / 255.0f;
            color_buffer[3] = clear_color.a / 255.0f;
            ImGui::ColorEdit4("clear color", (float*)&color_buffer);
            clear_color.r = color_buffer[0] * 255;
            clear_color.g = color_buffer[1] * 255;
            clear_color.b = color_buffer[2] * 255;
            clear_color.a = color_buffer[3] * 255;
        }

        ImGui::Text("中文测试");
        ImGui::PopFont();
    }

    ImGui::Separator();

    { // 显示纹理
        static int w, h = 0;

        ImGui::Text("图片测试");

        Texture* texture = resources_pool.get_texture_pool().at(ResourcesID::Tex_Test);
        SDL_QueryTexture(texture, nullptr, nullptr, (int*)&w, (int*)&h);
        ImGui::Image((ImTextureID)texture, ImVec2(w / 2, h / 2));
    }

    ImGui::PopFont();
    ImGui::End();
}

int
main()
{
    printf("Hello, World!\n");

    painter.Init("Hello, World!");
    resources_pool.LoadResources(painter.renderer);

    while(is_running)
    {
        painter.On_frame_begin(f);

        ImguiTestWindow();

        painter.On_frame_end(&clear_color);
    }

    painter.Quit();

    return 0;
}
