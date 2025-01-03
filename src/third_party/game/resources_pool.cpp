
// resources_pool.cpp

#include "resources_pool.h"

ResourcesPool* ResourcesPool::instance = nullptr;
ResourcesPool&
ResourcesPool::Instance()
{
    if(instance) return *instance;
    instance = new ResourcesPool();
    return *instance;
}

bool
ResourcesPool::LoadResources(Renderer* renderer)
{
    static ImGuiIO& io = ImGui::GetIO();

    { // 加载纹理
        texture_pool[ResourcesID::Tex_Test] = IMG_LoadTexture(renderer, "pictures/kana.jpg");
    }

    { // 加载音乐
        music_pool[ResourcesID::Music_Test] = Mix_LoadMUS("sounds/Phigros - いざ、参ります.flac");
    }

    { // 加载字体
        font_pool[ResourcesID::Font_default]           = io.Fonts->AddFontDefault();
        font_pool[ResourcesID::Font_ipix]              = io.Fonts->AddFontFromFileTTF("fonts/ipix.ttf", 12.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        font_pool[ResourcesID::Font_SmileySans]        = io.Fonts->AddFontFromFileTTF("fonts/SmileySans-Oblique.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        font_pool[ResourcesID::Font_SweiAliasLegCJKjp] = io.Fonts->AddFontFromFileTTF("fonts/SweiAliasLegCJKjp-Regular.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    }

    bool flag = true;

    // 检查加载纹理
    for(const auto& pair : texture_pool)
    {
        if(!pair.second)
        {
            printf("Error: IMG_LoadTexture(): %s\n", IMG_GetError());
            flag = false;
        }
    }

    // 检查加载音乐
    for(const auto& pair : music_pool)
    {
        if(!pair.second)
        {
            printf("Error: Mix_LoadMUS(): %s\n", Mix_GetError());
            flag = false;
        }
    }

    // 检查加载字体
    for(const auto& pair : font_pool)
    {
        if(!pair.second)
        {
            printf("Error: TTF_OpenFont(): %s\n", TTF_GetError());
            flag = false;
        }
    }

    return flag;
}

const ResourcesPool::FontPool&
ResourcesPool::get_font_pool() const
{
    return font_pool;
}

const ResourcesPool::SoundPool&
ResourcesPool::get_sound_pool() const
{
    return sound_pool;
}

const ResourcesPool::MusicPool&
ResourcesPool::get_music_pool() const
{
    return music_pool;
}

const ResourcesPool::TexturePool&
ResourcesPool::get_texture_pool() const
{
    return texture_pool;
}
