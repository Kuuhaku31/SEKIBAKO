
// game.cpp

#include "game.h"

#include "debuger.h"
#include "imgui_windows.h"
#include "resources_pool.h"

static Painter&       painter        = Painter::Instance();
static ResourcesPool& resources_pool = ResourcesPool::Instance();
static Debuger&       debuger        = Debuger::Instance();

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

        painter.DrawLine(0, 1, 0, Color{ 0xff, 0xff, 0xff, 0xFF });

        painter.DrawLine(-100, -100, 100, 100, Color{ 0xff, 0x00, 0xff, 0xFF });
        painter.DrawLine(-100, 100, 100, -100, Color{ 0xff, 0x00, 0xff, 0xFF });
        painter.DrawRect(100, 100, 100, 100, Color{ 0xff, 0xff, 0x00, 0xFF }, true);
        painter.DrawCircle(0, 0, 5, COLOR_GREEN, true);

        painter.DrawLine(-1.5, 0.5, 2, -0.5, COLOR_PURPLE);
        painter.DrawLine(-1.5, 0.5, 0, 0, COLOR_PURPLE);

        painter.DrawRect(0, 0, 1, 1, COLOR_LIGHT_GRAY, true);

        player->On_render();
    };

    game_view.Set_unit_size(50.f);

    player = new Player(Vector2{ 1, 3 }, COLOR_ORANGE);
    player->Set_mass(1.0f);
    player->Set_radius(0.5);
    player->Set_color(Color{ 0xff, 0x00, 0x00, 0xFF });
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
    painter.Init("Game", IRect{ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1200 });
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

        debuger.ImGuiWin_Debug(&is_open_debug_window);

        on_uodate_player(painter.imgui_io->DeltaTime);

        painter.On_frame_end(render_callback);
    }
}

void
Game::game_quit()
{
    delete player;

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
        if(ImGui::IsKeyDown(ImGuiKey_UpArrow)) move_dir += VECTOR2_UNIT_UP;
        if(ImGui::IsKeyDown(ImGuiKey_DownArrow)) move_dir += VECTOR2_UNIT_DOWN;
        if(ImGui::IsKeyDown(ImGuiKey_LeftArrow)) move_dir += VECTOR2_UNIT_LEFT;
        if(ImGui::IsKeyDown(ImGuiKey_RightArrow)) move_dir += VECTOR2_UNIT_RIGHT;

        move_dir.to_unit();
        move_dir /= (game_view.Get_unit_size() * 0.1f);
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
        if(ImGui::IsKeyDown(ImGuiKey_W)) move_dir += VECTOR2_UNIT_UP;
        if(ImGui::IsKeyDown(ImGuiKey_S)) move_dir += VECTOR2_UNIT_DOWN;
        if(ImGui::IsKeyDown(ImGuiKey_A)) move_dir += VECTOR2_UNIT_LEFT;
        if(ImGui::IsKeyDown(ImGuiKey_D)) move_dir += VECTOR2_UNIT_RIGHT;

        move_dir.to_unit();
        move_dir *= player_force;
        player->Force(move_dir);
        player->Force_resistance(player_friction, player_air_resistance);
    }
}

void
Game::on_update_view()
{
    static Vector2 view_size;

    view_size = Vector2{ painter.imgui_io->DisplaySize.x, painter.imgui_io->DisplaySize.y };
    view_size /= game_view.Get_unit_size();
    game_view.Set_view_size(view_size);
    game_view.Set_view_center_position(camera.Get_position());
}

void
Game::on_uodate_player(const float& delta_time)
{
    player->On_update(delta_time);

    player_hight = -player->Get_position().vy;
}
