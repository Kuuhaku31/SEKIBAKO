
// main.cpp

#include "graph.h"
#include "resources_pool.h"

#include "header.h"

#include <SDL_render.h>
#include <stdio.h>
#include <string>


void
渲染图片(const char* label)
{
    ResourcesPool& resources_pool = ResourcesPool::Instance();

    Texture* my_texture = resources_pool.Get_texture(label);
    if(my_texture)
    {
        // 获取纹理宽高
        IRect    size   = my_texture->get_size();
        uint32_t id     = my_texture->get_id();
        float    scale  = 0.3f;
        ImVec2   size_v = ImVec2(size.w * scale, size.h * scale);

        ImGui::Text("Succeed to load texture: %d", id);
        ImGui::Image((ImTextureID)(intptr_t)(id), size_v);
    }
    else
    {
        ImGui::Text("Failed to load texture!");
    }
}


int
main(int, char**)
{
    printf("Hello, TEST!\n");

    Vector2 vec(1.0f, 2.0f);
    printf("Vector2: (%f, %f)\n", vec.x, vec.y);

    Graph& graph = Graph::Instance();
    graph.Init("Test", IRect{ 100, 100, 1280, 720 });


    // Load resources
    ResourcesPool& resources_pool = ResourcesPool::Instance();
    resources_pool.LoadResources();

    // Our state
    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    // int my_image_w = 0;
    // int my_image_h = 0;

    // unsigned int my_texture = LoadTexture("./assets/test.jpg", &my_image_w, &my_image_h);

    // Main loop
    bool done = false;
    while(!done)
    {
        graph.On_frame_begin([&done, &graph](const Event& event) {
            // Handle events
            if(event.type == SDL_QUIT)
                done = true;
            if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == graph.GetGraphID())
                done = true;
        });

        ImGuiIO& io = ImGui::GetIO();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if(show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f       = 0.0f;
            static int   counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if(ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if(show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if(ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        static ImVec2 rect_pos;
        if(ImGui::Begin("Demo Window"))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2      p         = ImGui::GetCursorScreenPos();

            draw_list->AddRectFilled(ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 110, p.y + 60), IM_COL32(200, 100, 100, 255));
            draw_list->AddCircle(ImVec2(p.x + 60, p.y + 100), 30, IM_COL32(100, 200, 100, 255), 0, 3.0f);

            ImGui::DragFloat2("Rect Position", (float*)&rect_pos);

            // 渲染图片
            渲染图片("test_jpg");
            渲染图片("pink");
            渲染图片("老婆！");
        }
        ImGui::End();


        // 获取背景绘图列表
        ImDrawList* bg = ImGui::GetBackgroundDrawList();

        // ImVec2 win_pos  = ImGui::GetWindowPos();
        // ImVec2 win_size = ImGui::GetWindowSize();
        IRect graph_layout;
        graph.GetGraphLayout(graph_layout);

        // 计算屏幕中心
        ImVec2 center = ImVec2(graph_layout.w * 0.5f, graph_layout.h * 0.5f);
        // center.x += graph_layout.x, center.y += graph_layout.y;

        // 画一个半径 10 的绿色圆
        bg->AddRectFilled(rect_pos, ImVec2(rect_pos.x + 50, rect_pos.y + 50), IM_COL32(255, 0, 0, 255));
        bg->AddCircleFilled(center, 10.0f, IM_COL32(0, 255, 0, 255));

        debug_info info;
        DebugWindow(info);

        graph.On_frame_end([]() {
            // Render the frame
        });
    }

    // stbi_image_free(data);

    graph.Quit();

    resources_pool.FreeResources();

    return 0;
}
