

// debug.cpp

#include "header.h"

#include "graph.h"


void
DebugWindow(const debug_info& info)
{
    static Graph&   graph = Graph::Instance();
    static ImGuiIO& io    = ImGui::GetIO();


    ImGui::Begin("Debug");
    ImGui::Text("Debug information");

    ImGui::Text("Name: %s", info.name);
    ImGui::Text("Line: %d", info.line);
    ImGui::Text("File: %s", info.file);

    ImVec2 win_pos  = ImGui::GetWindowPos();
    ImVec2 win_size = ImGui::GetWindowSize();

    ImGui::Text("Window Position: (%.1f, %.1f)", win_pos.x, win_pos.y);
    ImGui::Text("Window Size: (%.1f, %.1f)", win_size.x, win_size.y);

    ImGui::Text("IO Display Size: (%.1f, %.1f)", io.DisplaySize.x, io.DisplaySize.y);
    ImGui::Text("IO Delta Time: (%.1f)", io.DeltaTime);

    IRect layout;
    graph.GetGraphLayout(layout);
    ImGui::Text("Window Rect: (%d, %d, %d, %d)", layout.x, layout.y, layout.w, layout.h);

    // 计算屏幕中心
    ImVec2 center = ImVec2(layout.w * 0.5f, layout.h * 0.5f);
    // center.x += layout.x, center.y += layout.y;
    ImGui::Text("Graph Center: (%.1f, %.1f)", center.x, center.y);

    ImGui::End();
}
