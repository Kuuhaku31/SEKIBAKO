
// debug.cpp

#include "imgui_windows.h"

#include "game.h"
#include "resources_pool.h"
#include "util.h"

void
ImGuiWin_Debug(bool* is_open)
{
    static Game&          game           = Game::Instance();
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    if(is_open && !*is_open) return;

    ImGui::Begin("Hello, ImGuiWin_Debug!", is_open);
    ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SmileySans));

    {
        ImGui::Text("Hello, ImGuiWin_Debug!");
        ImGui::SameLine();
        ImGui::Text("中文测试");

        const Object& player = game.Get_player();

        ImGui::Text("Player Position: (%.2f, %.2f)", player.Get_position().vx, player.Get_position().vy);
        ImGui::Text("Player Velocity: (%.2f, %.2f)", player.Get_velocity().vx, player.Get_velocity().vy);
        ImGui::Text("Player Acceleration: (%.2f, %.2f)", player.Get_acceleration().vx, player.Get_acceleration().vy);
        ImGui::Text("Player Mass: %.2f", player.Get_mass());
        ImGui::InputFloat("Player Force", &game.player_force);
        ImGui::InputFloat("Player Friction", &game.player_friction);
        ImGui::InputFloat("Player Air Resistance", &game.player_air_resistance);
    }

    ImGui::Separator();

    {
        ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SweiAliasLegCJKjp));
        ImGui::Text("日本語テスト");

        // 显示帧率
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f", io.Framerate);
        ImGui::SameLine();
        ImGui::Text("Frame Time: %.2f ms", 1000.0f / io.Framerate);

        ImGui::PopFont();
    }

    ImGui::Separator();

    {
        ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_ipix));
        // 调整颜色
        {
            static float color_buffer[4] = { 0 };

            color_to_float4(game.clear_color, color_buffer);
            ImGui::ColorEdit4("clear color", (float*)&color_buffer);
            float4_to_color((const float*)&color_buffer, game.clear_color);
        }

        ImGui::Text("中文测试");
        ImGui::PopFont();
    }

    ImGui::Separator();

    { // 播放音乐
        static bool is_playing = false;

        ImGui::Text("音乐测试");

        if(ImGui::Button(is_playing ? "Pause" : "Play"))
        {
            if(is_playing)
            {
                Mix_PauseMusic();
            }
            else
            {
                Mix_PlayMusic(resources_pool.get_music_pool().at(ResourcesID::Music_Test), -1);
            }

            is_playing = !is_playing;
        }
    }

    ImGui::Separator();

    { // 显示纹理
        static int   w, h = 0;
        static float scale = 0.5f; // 缩放比例

        ImGui::Text("图片测试");
        ImGui::DragFloat("Scale", &scale, 0.01f, 0.1f, 2.0f);

        Texture* texture = resources_pool.get_texture_pool().at(ResourcesID::Tex_Test);
        SDL_QueryTexture(texture, nullptr, nullptr, (int*)&w, (int*)&h);
        ImGui::Image((ImTextureID)texture, ImVec2(w * scale, h * scale));
    }

    ImGui::PopFont();
    ImGui::End();
}
