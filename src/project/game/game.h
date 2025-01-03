
// game.h

#pragma once

#include "imgui_setup.h"

#include <stdint.h>

class Game
{
public:
    static Game& Instance();

public:
    int PlayGame();

public:
    Color clear_color;

private:
    bool is_running = true;

    bool is_open_debug_window = false;

private:
    void game_init();
    void game_loop();
    void game_quit();

    void input_event();

    EventCallback    event_callback;
    RendererCallback render_callback;

    Object camera;
    View   game_view;

private:
    Game();
    ~Game()                      = default;
    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;

    static Game* instance;
};
