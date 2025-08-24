
// main.cpp

#include "graph.h"
#include "resources_pool.h"

#include "header.h"

#include <SDL_render.h>
#include <stdio.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/gl.h>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"


// Simple implementation of LoadTextureFromFile using stb_image and OpenGL
// 用 stb_image 加载图片并用 OpenGL 创建纹理，返回纹理ID
GLuint
LoadTextureFromFile(const char* filename, int* out_width, int* out_height)
{
    // 1. 用 stb_image 加载图片数据，channels=4 强制输出 RGBA 格式
    int            channels = 0;
    unsigned char* data     = stbi_load(filename, out_width, out_height, &channels, 4);
    if(!data)
    {
        // 加载失败，输出错误信息
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        return 0;
    }

    // 2. 生成一个 OpenGL 纹理对象
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // 3. 设置纹理参数（线性过滤）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. 上传像素数据到 GPU，格式为 RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *out_width, *out_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // 5. 释放 CPU 端图片数据
    stbi_image_free(data);

    // 6. 返回 OpenGL 纹理ID
    return texture_id;
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


    int    my_image_w = 0;
    int    my_image_h = 0;
    GLuint my_texture = LoadTextureFromFile("./assets/test.jpg", &my_image_w, &my_image_h);

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

            if(my_texture)
            {
                ImGui::Text("Succeed to load texture: %d", my_texture);
                ImGui::Image((ImTextureID)(intptr_t)my_texture, ImVec2((float)my_image_w, (float)my_image_h));
            }
            else
                ImGui::Text("Failed to load texture!");
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
    glDeleteTextures(1, &my_texture);

    return 0;
}
