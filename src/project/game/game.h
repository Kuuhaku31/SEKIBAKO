
// game.h

#pragma once

#include "imgui_setup.h"

#include "player.h"

#include <stdint.h>

class Game
{
    friend class Debuger;

public:
    static Game& Instance();

public:
    int PlayGame();

public:
    Color clear_color;

public:
    float player_hight = 0.0f; // 角色物理高度

private:
    bool is_running = true;

    bool is_open_debug_window = false;

private:
    void game_init();
    void game_loop();
    void game_quit();

    void input_event();

    void on_update_view();
    void on_uodate_player(const float& delta_time);

    EventCallback    event_callback;
    RendererCallback render_callback;

    Object camera;
    View   game_view;

    Player* player = nullptr;

private:
    Game();
    ~Game()                      = default;
    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;

    static Game* instance;
};
