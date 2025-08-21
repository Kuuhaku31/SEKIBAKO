# ============================================================
# Makefile for cpp-sdl2 + imgui project (MSYS2 mingw64)
# 
# Purpose: Build SDL2 + ImGui C++ project on Windows/MSYS2
# ============================================================

# ---------------------------
# 路径设置
# ---------------------------

# └─src
#     ├─project
#     │  ├─debug
#     │  ├─game
#     │  │  ├─animation_master
#     │  │  ├─collision_manager
#     │  │  └─effect_master
#     │  ├─imgui_windows
#     │  └─player
#     │      ├─player_effects
#     │      └─player_states
#     ├─test
#     │  └─test_base
#     └─third_party
#         ├─animation
#         ├─base
#         ├─imgui_setup
#         ├─resources
#         └─state_machine

build_dir = ./build


imgui_dir = D:/repositories/github/imgui
imgui_src := \
	$(imgui_dir)/imgui.cpp \
	$(imgui_dir)/imgui_demo.cpp \
	$(imgui_dir)/imgui_draw.cpp \
	$(imgui_dir)/imgui_tables.cpp \
	$(imgui_dir)/imgui_widgets.cpp \
	$(imgui_dir)/backends/imgui_impl_sdl2.cpp \
	$(imgui_dir)/backends/imgui_impl_sdlrenderer2.cpp
# 	$(imgui_dir)/backends/imgui_impl_opengl3.cpp
imgui_include_path := \
	$(imgui_dir) \
	$(imgui_dir)/backends \


third_party_dir = ./src/third_party
third_party_src := $(shell find $(third_party_dir) -name "*.c" -o -name "*.cpp") # 获取子文件夹下所有c cpp文件
third_party_include_path := $(shell find $(third_party_dir) -type d)

project_dir = ./src/project
project_src := $(shell find $(project_dir) -name "*.c" -o -name "*.cpp")
project_include_path := $(shell find $(project_dir) -type d)


imgui_objs := $(imgui_src)
imgui_objs := $(subst $(imgui_dir), , $(imgui_objs))
imgui_objs := $(addprefix $(build_dir)/imgui, $(imgui_objs))
imgui_objs := $(addsuffix .o, $(imgui_objs))


third_party_objs := $(third_party_src)
third_party_objs := $(subst $(third_party_dir), , $(third_party_objs))
third_party_objs := $(addprefix $(build_dir)/third_party, $(third_party_objs))
third_party_objs := $(addsuffix .o,$(third_party_objs))


project_objs := $(project_src)
project_objs := $(subst $(project_dir), , $(project_objs))
project_objs := $(addprefix $(build_dir)/project, $(project_objs))
project_objs := $(addsuffix .o, $(project_objs))


SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDLIBS := $(shell pkg-config --libs sdl2)
SDL_LDLIBS := $(subst -mwindows, , $(SDL_LDLIBS))
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -g

msg:
	@echo "$(imgui_src)"
	@echo "$(imgui_include_path)"
	@echo =====================
	@echo "$(third_party_src)"
	@echo "$(third_party_include_path)"
	@echo =====================
	@echo "$(SDL_CFLAGS)"
	@echo "$(SDL_LDLIBS)"
	@echo =====================
	@echo [imgui_objs] "$(imgui_objs)"
	@echo [third_party_objs] "$(third_party_objs)"
	@echo [project_objs] "$(project_objs)"


# 编译规则
$(build_dir)/imgui/%.cpp.o: $(imgui_dir)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(imgui_include_path)) -c -o $@ $< $(SDL_CFLAGS)

$(build_dir)/third_party/%.cpp.o: $(third_party_dir)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(third_party_include_path) $(imgui_include_path)) -c -o $@ $<

$(build_dir)/project/%.cpp.o: $(project_dir)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(project_include_path) $(third_party_include_path) $(imgui_include_path)) -c -o $@ $<

# app 目标
app: $(imgui_objs) $(third_party_objs) $(project_objs)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $^ $(addprefix -I, $(project_include_path) $(third_party_include_path) $(imgui_include_path)) $(SDL_LDLIBS) $(SDL_CFLAGS) -lcjson -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL_gfx

clear:
	@rm -rf $(build_dir)


update-assets:
	@rm -rf bin/assets
	@mkdir -p bin/assets
	@cp -r D:/OneDrive/resources/SEKIBAKO/* bin/assets/
