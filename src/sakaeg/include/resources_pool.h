
// resources_pool.h

#pragma once

#include "base.h"

#include "imgui.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


// typedef SDL_Texture  Texture;
// typedef SDL_Renderer Renderer;
// Mix_Chunk：用于存储短小的音效（如按钮点击、爆炸声等），通常是一次性加载到内存中，适合播放短促、频繁的声音。底层一般是 WAV、OGG 等格式，支持多次同时播放（多通道）。
// Mix_Music：用于存储较长的音乐（如背景音乐 BGM），通常是流式读取，不会一次性全部加载到内存，适合播放长时间的音频。支持的格式有 MP3、OGG、MIDI 等，通常一次只能播放一首。
typedef ImFont    Font;
typedef Mix_Chunk Sound;
typedef Mix_Music Music;


// 纹理类
typedef class Texture
{
public:
    Texture(const char* filename);
    ~Texture();

    uint32_t     get_id() const;
    const IRect& get_size() const;

private:
    uint32_t id;
    IRect    size;
}* Texture_ptr;

// 位置修正回调函数
typedef std::function<void(float& dst_x, float& dst_y, const float& src_w, const float& src_h)> CorrectivePos;

// 渲染回调函数
typedef std::function<void(const Texture* tex, const IRect& src, const IRect& dst)> RenderCallback;

// 动画信息
struct AnimationInformation
{
    // 必须设置的参数
    Texture*  texture;        // 纹理
    uint16_t  num_x;          // 纹理横向切成多少帧
    uint16_t  num_y;          // 纹理纵向切成多少帧
    uint16_t* frame_idx_list; // 帧索引列表
    uint16_t  frame_count;    // 帧数

    // 可选参数
    float frame_interval = 0.0f;  // 帧间隔
    float angle          = 0.0f;  // 渲染角度
    float texs_size      = 0.0f;  // 渲染大小，对于这个纹理，一个单位长度等于 texs_size 个像素
    bool  is_loop        = false; // 是否循环播放

    CorrectivePos on_corrective; // 位置修正
};

// 动画模板
class AnimationTemplate
{
    friend class AnimationInstance;

public:
    AnimationTemplate(const AnimationInformation& info);
    ~AnimationTemplate();

private:
    Texture* texture;        // 纹理
    Point*   frame_src_list; // 帧源列表
    uint16_t frame_w;        // 帧宽
    uint16_t frame_h;        // 帧高
    uint16_t frame_count;    // 帧数

    float frame_interval = 0.1f;  // 帧间隔
    float angle          = 0.0f;  // 渲染角度
    float texs_size      = 1.0f;  // 渲染大小，对于这个纹理，一个单位长度等于 texs_size 个像素
    bool  is_loop        = false; // 是否循环播放

    CorrectivePos on_corrective; // 位置修正
};

// 动画实例
class AnimationInstance : public Vector2
{
public:
    AnimationInstance(const AnimationTemplate& animation, Callback f = nullptr);
    ~AnimationInstance() = default;

public:
    void On_render() const;           // 渲染
    void On_update(float delta_time); // 更新

    void GetCurrentFrame(Texture_ptr* tex, IRect* src_rect, FRect* dst_rect) const;

public:
    void Restart();              // 重置
    void Set_play_time(float t); // 设置播放时间

    void Set_frame_interval(float interval);
    void Set_frame_interval_add(float interval);
    void Set_frame_interval_mul(float interval);
    void Set_size(float size);
    void Set_size_add(float size);
    void Set_size_mul(float size);

    void Set_on_corrective(CorrectivePos f);
    void Set_on_finished(Callback f);

public:
    const float& Get_ph_w() const;    // 物理宽
    const float& Get_ph_h() const;    // 物理高
    const bool&  Is_finished() const; // 动画是否结束

public:
    float angle; // 渲染角度

private:
    const AnimationTemplate& animation;

    Timer    frame_timer;           // 帧计时器
    uint16_t frame_current = 0;     // 当前帧
    bool     is_finished   = false; // 动画是否结束

    float texs_size; // 渲染大小
    float ph_w;      // 物理宽
    float ph_h;      // 物理高

    CorrectivePos on_corrective; // 位置修正

    Callback on_finished; // 动画结束回调


private:
    inline void update_ph_vy();
};


class ResourcesPool
{
public:
    static ResourcesPool& Instance();

public:
    typedef std::unordered_map<std::string, Texture*>           TexturePool;
    typedef std::unordered_map<std::string, Font*>              FontPool;
    typedef std::unordered_map<std::string, Sound*>             SoundPool;
    typedef std::unordered_map<std::string, Music*>             MusicPool;
    typedef std::unordered_map<std::string, AnimationTemplate*> AnimationPool;

public:
    bool LoadResources();
    bool FreeResources();

    Texture*           Get_texture(std::string id) const;
    Font*              Get_font(std::string id) const;
    Sound*             Get_sound(std::string id) const;
    Music*             Get_music(std::string id) const;
    AnimationTemplate* Get_animation(std::string id) const;

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
