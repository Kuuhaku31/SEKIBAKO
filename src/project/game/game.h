
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

private:
    void game_init();
    void game_loop();
    void game_quit();

    EventCallback event_callback;

private:
    Game();
    ~Game()                      = default;
    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;

    static Game* instance;
};
