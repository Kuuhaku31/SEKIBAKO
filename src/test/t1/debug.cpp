

// debug.cpp

#include "header.h"

#include "graph.h"
#include "resources_pool.h"


void
DebugWindow(const debug_info& info)
{
    static Graph&   graph = Graph::Instance();
    static ImGuiIO& io    = ImGui::GetIO();


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

        ImGui::Text("Succeed to load texture: %d", id);
        ImGui::Image((ImTextureID)(intptr_t)(id), size_v);
    }
    else
    {
        ImGui::Text("Failed to load texture!");
    }
}


Mix_Music* gMusic    = nullptr;
bool       isPlaying = false;
int        volume    = MIX_MAX_VOLUME / 2;

void
ShowMusicPlayerUI()
{
    ImGui::Begin("Music Player");

    if(ImGui::Button("Load Music"))
    {
        // 初始化 SDL_mixer
        int flags   = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
        int initted = Mix_Init(flags);
        if((initted & flags) != flags)
        {
            // std::cerr << "Missing codec support: " << Mix_GetError() << std::endl;
            printf("Error: Mix_Init(): %s\n", Mix_GetError());
        }
        else
        {
            // SDL_mixer 初始化成功
            printf("SDL_mixer initialized successfully\n");
        }

        if(gMusic) Mix_FreeMusic(gMusic);
        gMusic = Mix_LoadMUS("./assets/Hello.wav");
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

    ImGui::End();
}
