
// imgui_setup.h

#pragma once

#include "base.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

typedef SDL_Event    Event;
typedef SDL_Color    Color;
typedef SDL_Texture  Texture;
typedef SDL_Renderer Renderer;
typedef ImFont       Font;
typedef Mix_Chunk    Sound;
typedef Mix_Music    Music;

typedef std::function<void(const Event&)> EventCallback;

// Painter
class Painter
{
    friend class Config;

public:
    static Painter& Instance();

    int Init(const char* title = nullptr, const IRect* layout = nullptr); // 0:初始化成功
    int Quit();

    void On_frame_begin(EventCallback f = nullptr) const;
    void On_frame_end(const Color* clear_color = nullptr) const;

public:
    float Get_delta_time() const;
    float Get_frame_rate() const;
    float Get_unit_size() const;

public:
    bool Make_message_box(const char* title, const char* message) const;

public:
    void     Set_resorces_path(const char* file_path);
    Texture* LoadTexture(const char* file_path) const;
    Sound*   LoadWAV(const char* file_path) const;
    Music*   LoadMUS(const char* file_path) const;

private:
    char resources_path[_MAX_PATH] = "";

public:
    void Create_texture(Texture*& texture, int tex_wide, int tex_high) const;
    void Destroy_texture(Texture*& texture) const;

    void Render_target(Texture* texture = nullptr, const View* view = nullptr);
    void Render_color(int color) const;
    void Render_clear(int color) const;
    void Render_clear() const;

private:
    void draw_plaid_line(int p) const;

public:
    void Draw_plaid() const;

    void DrawLine(float A, float B, float C, int color) const; // 直线方程 Ax + By + C = 0
    void DrawLine_(float start_x, float start_y, float end_x, float end_y, int color) const;
    void DrawLine(float start_x, float start_y, float end_x, float end_y, int color) const;
    void DrawArc(float center_x, float center_y, float radius, float start_angle, float end_angle, int color) const;
    void DrawCircle(float center_x, float center_y, float radius, int color, bool is_solid = true) const;
    void DrawRect(float x, float y, float w, float h, int color, bool is_solid = true) const;
    void DrawTriangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, int color, bool is_solid = true) const;

    void DrawTexture(Texture* texture, const IRect& rect_src, const IRect& rect_dst, float angle) const;

private:
    int init_flag = 1; // 1:未初始化 0:初始化成功 -1:初始化失败

public:
    SDL_Window* window   = nullptr;
    Renderer*   renderer = nullptr;
    ImGuiIO*    imgui_io = nullptr;

private:
    const View* painter_view = nullptr;

private: // 单例模式
    Painter()                          = default;
    ~Painter()                         = default;
    Painter(const Painter&)            = delete;
    Painter& operator=(const Painter&) = delete;

    static Painter* instance;
};
