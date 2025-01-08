
// resources_pool.h

#pragma once

#include <string>
#include <unordered_map>

typedef class SDL_Texture Texture;
typedef class ImFont      Font;
typedef class Mix_Chunk   Sound;
typedef struct _Mix_Music Music;
class Animation;

class ResourcesPool
{
public:
    static ResourcesPool& Instance();

public:
    typedef std::unordered_map<std::string, Texture*>   TexturePool;
    typedef std::unordered_map<std::string, Font*>      FontPool;
    typedef std::unordered_map<std::string, Sound*>     SoundPool;
    typedef std::unordered_map<std::string, Music*>     MusicPool;
    typedef std::unordered_map<std::string, Animation*> AnimationPool;

public:
    bool LoadResources();
    bool FreeResources();

    Texture*   Get_texture(std::string id) const { return texture_pool.at(id); }
    Font*      Get_font(std::string id) const { return font_pool.at(id); }
    Sound*     Get_sound(std::string id) const { return sound_pool.at(id); }
    Music*     Get_music(std::string id) const { return music_pool.at(id); }
    Animation* Get_animation(std::string id) const { return animation_pool.at(id); }

private:
    FontPool      font_pool;
    SoundPool     sound_pool;
    MusicPool     music_pool;
    TexturePool   texture_pool;
    AnimationPool animation_pool;

private: // 单例模式
    ResourcesPool()                                = default;
    ~ResourcesPool()                               = default;
    ResourcesPool(const ResourcesPool&)            = delete;
    ResourcesPool& operator=(const ResourcesPool&) = delete;

    static ResourcesPool* instance;
};
