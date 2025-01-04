
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
    clear_color = { 0x33, 0x33, 0x33, 0xFF };

    event_callback = [this](const Event& e) {
        if(e.type == SDL_QUIT) is_running = false;
    };

    render_callback = [this]() {
        painter.Render_clear(clear_color);
        painter.Render_target(nullptr, &game_view);

        painter.DrawLine(0, 1, 0, 0xffffffFF);

        painter.DrawLine(-100, -100, 100, 100, 0xff00ffFF);
        painter.DrawLine(-100, 100, 100, -100, 0xff00ffFF);
        painter.DrawRect(100, 100, 100, 100, 0xffff00FF, true);
        painter.DrawCircle(0, 0, 50, 0x00ff00FF, true);

        player.On_render();
    };

    player.Set_mass(1.0f);
    player.Set_radius(10);
    player.Set_color(0xff0000FF);
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

        input_event();

        on_update_view();

        ImGuiWin_Debug(&is_open_debug_window);

        player.On_update(painter.imgui_io->DeltaTime);

        painter.On_frame_end(render_callback);
    }
}

void
Game::game_quit()
{
    resources_pool.FreeResources();
    painter.Quit();
}

void
Game::input_event()
{
    if(ImGui::IsKeyPressed(ImGuiKey_P)) is_open_debug_window = !is_open_debug_window;

    static Vector2 move_dir;

    { // 控制摄像机
        move_dir.to_zero();
        if(ImGui::IsKeyDown(ImGuiKey_UpArrow)) move_dir += { 0, -1 };
        if(ImGui::IsKeyDown(ImGuiKey_DownArrow)) move_dir += { 0, 1 };
        if(ImGui::IsKeyDown(ImGuiKey_LeftArrow)) move_dir += { -1, 0 };
        if(ImGui::IsKeyDown(ImGuiKey_RightArrow)) move_dir += { 1, 0 };

        move_dir.to_unit();
        move_dir /= game_view.Get_unit_size() * 0.5f;
        camera.Move(move_dir);

        // 控制摄像机的视野大小
        // , .
        float size = game_view.Get_unit_size();
        if(ImGui::IsKeyDown(ImGuiKey_Comma)) size *= 0.99f;
        if(ImGui::IsKeyDown(ImGuiKey_Period)) size *= 1.01f;
        game_view.Set_unit_size(size);
    }

    { // 控制玩家
        move_dir.to_zero();
        if(ImGui::IsKeyDown(ImGuiKey_W)) move_dir += { 0, -1 };
        if(ImGui::IsKeyDown(ImGuiKey_S)) move_dir += { 0, 1 };
        if(ImGui::IsKeyDown(ImGuiKey_A)) move_dir += { -1, 0 };
        if(ImGui::IsKeyDown(ImGuiKey_D)) move_dir += { 1, 0 };

        move_dir.to_unit();
        move_dir *= player_force;
        player.Force(move_dir);
        player.Force_resistance(player_friction, player_air_resistance);
    }
}

void
Game::on_update_view()
{
    Vector2 view_size = { painter.imgui_io->DisplaySize.x, painter.imgui_io->DisplaySize.y };
    view_size /= game_view.Get_unit_size();
    game_view.Set_view_size(view_size);
    game_view.Set_view_center_position(camera.Get_position());
}
