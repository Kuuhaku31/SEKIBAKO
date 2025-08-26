

// debug.cpp

#include "header.h"

#include "graph.h"
#include "resources_pool.h"

void
DebugWindow(const debug_info& info)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();
    static Graph&         graph          = Graph::Instance();
    static ImGuiIO&       io             = ImGui::GetIO();

    Font* my_font = resources_pool.Get_font("Sarasa-SuperTTC-x16");
    if(my_font)
    {
        ImGui::PushFont(my_font);
    }

    ImGui::Begin("Debug");
    ImGui::Text("Debug information");

    ImGui::Text("Name: %s", info.name);
    ImGui::Text("Line: %d", info.line);
    ImGui::Text("File: %s", info.file);

    ImVec2 win_pos  = ImGui::GetWindowPos();
    ImVec2 win_size = ImGui::GetWindowSize();

    ImGui::Text("Window Position: (%.1f, %.1f)", win_pos.x, win_pos.y);
    ImGui::Text("Window Size: (%.1f, %.1f)", win_size.x, win_size.y);

    ImGui::Text("IO Display Size: (%.1f, %.1f)", io.DisplaySize.x, io.DisplaySize.y);
    ImGui::Text("IO Delta Time: (%.1f)", io.DeltaTime);

    IRect layout;
    graph.GetGraphLayout(layout);
    ImGui::Text("Window Rect: (%d, %d, %d, %d)", layout.x, layout.y, layout.w, layout.h);

    // 计算屏幕中心
    ImVec2 center = ImVec2(layout.w * 0.5f, layout.h * 0.5f);
    // center.x += layout.x, center.y += layout.y;
    ImGui::Text("Graph Center: (%.1f, %.1f)", center.x, center.y);

    ImGui::End();

    if(my_font) ImGui::PopFont();
}


void
渲染图片(const char* label)
{
    ResourcesPool& resources_pool = ResourcesPool::Instance();

    Texture* my_texture = resources_pool.Get_texture(label);
    if(my_texture)
    {
        // 获取纹理宽高
        IRect    size   = my_texture->get_size();
        uint32_t id     = my_texture->get_id();
        float    scale  = 0.3f;
        ImVec2   size_v = ImVec2(size.w * scale, size.h * scale);

        // ImGui::Text("Succeed to load texture: %d", id);
        ImGui::Image((ImTextureID)(intptr_t)(id), size_v);
    }
    else
    {
        ImGui::Text("Failed to load texture!");
    }
}


void
播放音乐(const char* label)
{
    static ResourcesPool& resources_pool = ResourcesPool::Instance();

    static Music* gMusic    = nullptr;
    static bool   isPlaying = false;
    static int    volume    = MIX_MAX_VOLUME * 0.1f;


    if(ImGui::Button("Load Music"))
    {
        gMusic = resources_pool.Get_music(label);
        if(!gMusic)
        {
            printf("Load error: %s\n", Mix_GetError());
        }
    }

    if(ImGui::Button("Play"))
    {
        if(gMusic)
        {
            Mix_PlayMusic(gMusic, -1);
            isPlaying = true;
        }
        else
        {
            printf("Music not loaded\n");
        }
    }

    ImGui::SameLine();
    if(ImGui::Button("Pause"))
    {
        Mix_PauseMusic();
        isPlaying = false;
    }

    ImGui::SameLine();
    if(ImGui::Button("Resume"))
    {
        Mix_ResumeMusic();
        isPlaying = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Stop"))
    {
        Mix_HaltMusic();
        isPlaying = false;
    }

    // 音量滑条
    ImGui::SliderInt("Volume", &volume, 0, MIX_MAX_VOLUME);
    Mix_VolumeMusic(volume);

    ImGui::Text("Status: %s", isPlaying ? "Playing" : "Stopped");
}


void
画背景(const debug_info& info)
{
    static Graph& graph = Graph::Instance();

    // 获取背景绘图列表
    ImDrawList* bg = ImGui::GetBackgroundDrawList();

    // ImVec2 win_pos  = ImGui::GetWindowPos();
    // ImVec2 win_size = ImGui::GetWindowSize();
    IRect graph_layout;
    graph.GetGraphLayout(graph_layout);

    // 计算屏幕中心
    ImVec2 center = ImVec2(graph_layout.w * 0.5f, graph_layout.h * 0.5f);
    // center.x += graph_layout.x, center.y += graph_layout.y;

    // 画一个半径 10 的绿色圆
    bg->AddRectFilled(info.rect_pos, ImVec2(info.rect_pos.x + 50, info.rect_pos.y + 50), IM_COL32(255, 0, 0, 255));
    bg->AddCircleFilled(center, 10.0f, IM_COL32(0, 255, 0, 255));
}

void
画前景(const debug_info& info)
{
    // static Graph& graph = Graph::Instance();

    // 获取前景绘图列表
    ImDrawList* fg = ImGui::GetForegroundDrawList();

    // 画一个半径 10 的蓝色圆
    fg->AddCircleFilled(info.rect_pos, 10.0f, IM_COL32(0, 0, 255, 255 * 0.8f));
    // fg->AddImage((ImTextureID)(intptr_t)1, ImVec2(50, 50), ImVec2(70, 70));
}
