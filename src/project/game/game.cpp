
// game.cpp

#include "game.h"

#include "imgui_windows.h"
#include "resources_pool.h"

static Painter&       painter        = Painter::Instance();
static ResourcesPool& resources_pool = ResourcesPool::Instance();

Game* Game::instance = nullptr;
Game&
Game::Instance()
{
    if(instance) return *instance;
    instance = new Game();
    return *instance;
}

Game::Game()
{
    clear_color = { 0xcc, 0xcc, 0xcc, 0xFF };

    event_callback = [this](const Event& e) {
        if(e.type == SDL_QUIT) is_running = false;
    };
}

int
Game::PlayGame()
{
    game_init();
    game_loop();
    game_quit();

    return 0;
}

void
Game::game_init()
{
    painter.Init("Game");
    resources_pool.LoadResources(painter.renderer);
}

void
Game::game_loop()
{
    while(is_running)
    {
        painter.On_frame_begin(event_callback);

        {
            ImGuiWin_Debug();
        }

        painter.On_frame_end(&clear_color);
    }
}

void
Game::game_quit()
{
    resources_pool.FreeResources();
    painter.Quit();
}
