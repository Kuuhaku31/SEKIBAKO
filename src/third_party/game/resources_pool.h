
// resources_pool.h

#pragma once

#include "imgui_setup.h"

#include <unordered_map>

enum class ResourcesID
{
    // 纹理
    Tex_Test, // 测试纹理

    // 特效

    // 音效

    // 音乐

    // 字体
    Font_default,          // 默认字体
    Font_ipix,             // ipix
    Font_SmileySans,       // 得意黑
    Font_SweiAliasLegCJKjp // 日文字体
};

class ResourcesPool
{
public:
    static ResourcesPool& Instance();

public:
    typedef std::unordered_map<ResourcesID, Font*>    FontPool;
    typedef std::unordered_map<ResourcesID, Sound*>   SoundPool;
    typedef std::unordered_map<ResourcesID, Music*>   MusicPool;
    typedef std::unordered_map<ResourcesID, Texture*> TexturePool;

public:
    bool LoadResources(Renderer* renderer);

    const FontPool&    get_font_pool() const;
    const SoundPool&   get_sound_pool() const;
    const MusicPool&   get_music_pool() const;
    const TexturePool& get_texture_pool() const;

private:
    FontPool    font_pool;
    SoundPool   sound_pool;
    MusicPool   music_pool;
    TexturePool texture_pool;

private: // 单例模式
    ResourcesPool()                                = default;
    ~ResourcesPool()                               = default;
    ResourcesPool(const ResourcesPool&)            = delete;
    ResourcesPool& operator=(const ResourcesPool&) = delete;

    static ResourcesPool* instance;
};
