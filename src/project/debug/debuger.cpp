
// debuger.cpp

#include "debuger.h"

#include "game.h"
#include "resources_pool.h"
#include "util.h"

#include <bitset>

Debuger* Debuger::instance = nullptr;
Debuger&
Debuger::Instance()
{
    if(instance) return *instance;
    instance = new Debuger();
    return *instance;
}

void
Debuger::ImGuiWin_Debug(bool* is_open)
{
    static Game&          game           = Game::Instance();
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    if(is_open && !*is_open) return;

    ImGui::Begin("Hello, ImGuiWin_Debug!", is_open);
    ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SmileySans));
    ImGui::Text("Hello, ImGuiWin_Debug!");
    ImGui::SameLine();
    ImGui::Text("中文测试");

    ImGui::Separator();
    ImGui::PushFont(resources_pool.get_font_pool().at(ResourcesID::Font_SweiAliasLegCJKjp));

    {
        Player* player = game.player;
        if(player)
        {
            ImGui::Text("Player Controler: %s", std::bitset<32>(player->player_controler).to_string().c_str());
            ImGui::Text("Player State: %s", player->Current_state());
            ImGui::Text("Player Hight: %.2f", game.player_hight);
            ImGui::Text("Player Position: (%.2f, %.2f)", player->Get_position().vx, player->Get_position().vy);
            ImGui::Text("Player Velocity: (%.2f, %.2f)", player->Get_velocity().vx, player->Get_velocity().vy);
            ImGui::Text("Player Acceleration: (%.2f, %.2f)", player->Get_acceleration().vx, player->Get_acceleration().vy);
            ImGui::Text("Player Is Use Friction: %s", player->is_use_friction ? "true" : "false");
            ImGui::Text("Player Is Use Air Resistance: %s", player->is_use_air_resistance ? "true" : "false");
            ImGui::Text("Player Can Jump Count: %d", player->can_jump_count);
            ImGui::Text("Player Mass: %.2f", player->Get_mass());
            ImGui::InputFloat("Player Walk Acceleration", &player->walk_acceleration);
            ImGui::InputFloat("Player Run Acceleration", &player->run_acceleration);
            ImGui::InputFloat("Player Jump Force", &player->jump_acceleration);
            ImGui::InputFloat("Player Jump Time", &player->jump_time);
            ImGui::InputFloat("Player Roll Force", &player->roll_acceleration);
            ImGui::InputFloat("Player Roll Time", &player->roll_time);
            ImGui::InputFloat("Player Roll Time CD", &player->roll_cd);

            ImGui::Separator();
            ImGui::InputFloat("Player Attack Action Time", &player->attack_action_time);
            ImGui::InputFloat("Player Attack Effect Wait Time", &player->attack_effect_wait_time);
            ImGui::InputFloat("Player Attack Effect Time", &player->attack_effect_time);
            ImGui::InputFloat("Player Attack CD", &player->attack_cd);
            ImGui::Separator();

            ImGui::InputFloat("Player Dash Acceleration", &player->dash_acceleration);
            ImGui::InputFloat("Player Dash Min Speed", &player->dash_min_speed);
            ImGui::InputFloat("Player Friction", &player->player_friction);
            ImGui::InputFloat("Player Air Resistance", &player->player_air_resistance);
            ImGui::InputFloat("Player Friction Walk", &player->player_friction_walk);
            ImGui::InputFloat("Player Air Resistance Walk", &player->player_air_resistance_walk);
        }
    }

    ImGui::Separator();

    { // 视野参数
        const View& view = game.game_view;
        ImGui::Text("一单位距离显示为: %.2f个像素", view.Get_unit_size());
        ImGui::Text("View Center Position: (%.2f, %.2f)", view.Get_view_center_position().vx, view.Get_view_center_position().vy);
    }

    ImGui::Separator();

    {

        ImGui::Text("日本語テスト");

        // 显示帧率
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f", io.Framerate);
        ImGui::SameLine();
        ImGui::Text("Frame Time: %.2f ms", 1000.0f / io.Framerate);
    }

    ImGui::PopFont();
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
