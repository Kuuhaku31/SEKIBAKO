
// graph.cpp

#include "graph.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>


SDL_GLContext gl_context = nullptr;
SDL_Window*   window     = nullptr;

Color   clear_color       = COLOR_GRAY;
uint8_t clear_color_alpha = 255;

// 单例
Graph* Graph::instance = nullptr;
Graph&
Graph::Instance()
{
    if(instance) return *instance;
    return *(instance = new Graph());
}


int32_t
Graph::Init(const char* graph_title, const IRect& graph_layout)
{
    // 设置控制台 IO 编码为 UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // Setup SDL
    uint32_t flags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
    flags |= SDL_INIT_AUDIO;
    if(SDL_Init(flags) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";                                     // 指定 opengl 版本号
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);                                  //
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);                          //
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);                          //

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float           main_scale   = ImGui_ImplSDL2_GetContentScaleForDisplay(0); // 获取主窗口缩放比例
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window                       = SDL_CreateWindow(graph_title, graph_layout.x, graph_layout.y, (int)(graph_layout.w * main_scale), (int)(graph_layout.h * main_scale), window_flags);
    if(window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    // 创建 OpenGL 上下文
    gl_context = SDL_GL_CreateContext(window);
    if(gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync // 启用垂直同步

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);         // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi         = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
    io.ConfigDpiScaleFonts     = true;       // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;       // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
    io.Fonts->AddFontDefault();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // 初始化 SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    flags       = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
    int initted = Mix_Init(flags);
    if((initted & flags) != flags)
    {
        // std::cerr << "Missing codec support: " << Mix_GetError() << std::endl;
        printf("Error: Mix_Init(): %s\n", Mix_GetError());
    }

    return 0;
}

int32_t
Graph::Quit()
{
    // Cleanup

    // SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void
Graph::On_frame_begin(EventCallback f) const
{
    Event event;
    while(SDL_PollEvent(&event))
    {
        f(event);
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

static float
cl(uint8_t c)
{
    return (float)c / 255.0f;
}

void
Graph::On_frame_end(Callback) const
{
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(cl(clear_color.r), cl(clear_color.g), cl(clear_color.b), cl(clear_color_alpha));
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

uint32_t
Graph::GetGraphID() const
{
    return SDL_GetWindowID(window);
}

void
Graph::GetGraphLayout(IRect& layout) const
{
    SDL_GetWindowPosition(window, &layout.x, &layout.y);
    SDL_GetWindowSize(window, &layout.w, &layout.h);
}
