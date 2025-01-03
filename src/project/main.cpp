
// main.cpp

#include <stdio.h>

#include "imgui_setup.h"

static Painter& painter = Painter::Instance();

bool is_running = true;

Color clear_color = { 0xcc, 0xcc, 0xcc, 0xFF };

std::function<void(const Event&)> f = [](const Event& e) {
    if(e.type == SDL_QUIT) is_running = false;
};

int
main()
{
    printf("Hello, World!\n");

    painter.Init("Hello, World!");

    while(is_running)
    {
        painter.On_frame_begin(f);


        painter.On_frame_end(&clear_color);
    }

    painter.Quit();

    return 0;
}
