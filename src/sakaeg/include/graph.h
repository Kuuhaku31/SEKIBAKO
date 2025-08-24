
// graph.h

// Graph

#include "base.h"

#include "imgui.h"
#include <SDL.h>

typedef SDL_Event Event;
// typedef SDL_Texture Texture;
// typedef SDL_Renderer Renderer;
// typedef ImFont Font;
// typedef Mix_Chunk    Sound;
// typedef Mix_Music    Music;

typedef std::function<void(Event&)> EventCallback; // 事件回调函数


class Graph
{
    friend class Config;

public:
    static Graph& Instance();

    int32_t Init(const char* title = nullptr, const IRect& layout = IRect{ 0, 0, 0, 0 }); // 0:初始化成功
    int32_t Quit();

    void On_frame_begin(EventCallback f = nullptr) const;
    void On_frame_end(Callback f = nullptr) const;

    uint32_t GetGraphID() const;
    void     GetGraphLayout(IRect& layout) const;

private: // 单例模式
    Graph()                        = default;
    ~Graph()                       = default;
    Graph(const Graph&)            = delete;
    Graph& operator=(const Graph&) = delete;

    static Graph* instance;
};
