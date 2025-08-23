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



# third_party_dir = ./src/third_party
# third_party_src := $(shell find $(third_party_dir) -name "*.c" -o -name "*.cpp") # 获取子文件夹下所有c cpp文件
# third_party_include_path := $(shell find $(third_party_dir) -type d)

# project_dir = ./src/project
# project_src := $(shell find $(project_dir) -name "*.c" -o -name "*.cpp")
# project_include_path := $(shell find $(project_dir) -type d)


# third_party_objs := $(third_party_src)
# third_party_objs := $(subst $(third_party_dir), , $(third_party_objs))
# third_party_objs := $(addprefix $(build_dir)/third_party, $(third_party_objs))
# third_party_objs := $(addsuffix .o,$(third_party_objs))


# project_objs := $(project_src)
# project_objs := $(subst $(project_dir), , $(project_objs))
# project_objs := $(addprefix $(build_dir)/project, $(project_objs))
# project_objs := $(addsuffix .o, $(project_objs))



BUILD_PATH = ./build


IMGUI_PATH := D:/repositories/github/imgui
IMGUI_BACKEND_PATH := $(IMGUI_PATH)/backends
IMGUI_OBJ_PATH := $(BUILD_PATH)/imgui
IMGUI_SRC := $(shell find $(IMGUI_PATH) -name "*.c" -o -name "*.cpp")
IMGUI_INCLUDE_PATH := $(IMGUI_PATH) $(IMGUI_BACKEND_PATH)

IMGUI_OBJ := $(IMGUI_SRC)
IMGUI_OBJ := $(subst $(IMGUI_PATH), , $(IMGUI_OBJ))
IMGUI_OBJ := $(addprefix $(IMGUI_OBJ_PATH), $(IMGUI_OBJ))
IMGUI_OBJ := $(addsuffix .o, $(IMGUI_OBJ))


imgui_target_src += imgui.cpp
imgui_target_src += imgui_demo.cpp
imgui_target_src += imgui_draw.cpp
imgui_target_src += imgui_tables.cpp
imgui_target_src += imgui_widgets.cpp
imgui_target_src += backends/imgui_impl_sdl2.cpp
# imgui_target_src += backends/imgui_impl_sdlrenderer2.cpp
imgui_target_src += backends/imgui_impl_opengl3.cpp

imgui_target_obj := $(imgui_target_src)
imgui_target_obj := $(subst $(IMGUI_PATH), , $(imgui_target_obj))
imgui_target_obj := $(addprefix $(IMGUI_OBJ_PATH)/, $(imgui_target_obj))
imgui_target_obj := $(addsuffix .o, $(imgui_target_obj))


SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDLIBS := $(shell pkg-config --libs sdl2)
# SDL_LDLIBS := $(subst -mwindows, , $(SDL_LDLIBS))

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -g

msg:
	@echo =====================
	@echo [IMGUI_PATH] $(IMGUI_PATH)
	@echo [IMGUI_BACKEND_PATH] $(IMGUI_BACKEND_PATH)
	@echo [IMGUI_OBJ_PATH] $(IMGUI_OBJ_PATH)
	@echo [IMGUI_SRC] $(IMGUI_SRC)
	@echo [IMGUI_INCLUDE_PATH] $(IMGUI_INCLUDE_PATH)
	@echo [IMGUI_OBJ] $(IMGUI_OBJ)
	@echo 
	@echo [imgui_target_src] $(imgui_target_src)
	@echo [imgui_target_obj] $(imgui_target_obj)
	@echo =====================
	@echo "$(CXX)"
	@echo [imgui_src] "$(imgui_src)"
	@echo [imgui_include_path] "$(imgui_include_path)"
	@echo =====================
	@echo [third_party_src] "$(third_party_src)"
	@echo [third_party_include_path] "$(third_party_include_path)"
	@echo =====================
	@echo [SDL_CFLAGS] "$(SDL_CFLAGS)"
	@echo [SDL_LDLIBS] "$(SDL_LDLIBS)"
	@echo =====================
	@echo [imgui_objs] "$(imgui_objs)"
	@echo [third_party_objs] "$(third_party_objs)"
	@echo [project_objs] "$(project_objs)"


# 编译规则
$(IMGUI_OBJ_PATH)/%.cpp.o: $(IMGUI_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(IMGUI_INCLUDE_PATH)) -c -o $@ $< $(SDL_CFLAGS)

$(BUILD_PATH)/third_party/%.cpp.o: $(THIRD_PARTY_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(THIRD_PARTY_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) -c -o $@ $<

$(BUILD_PATH)/project/%.cpp.o: $(PROJECT_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(PROJECT_INCLUDE_PATH) $(THIRD_PARTY_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) -c -o $@ $<

# app 目标
app: $(IMGUI_OBJ) $(THIRD_PARTY_OBJ) $(PROJECT_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $^ $(addprefix -I, $(PROJECT_INCLUDE_PATH) $(THIRD_PARTY_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) $(SDL_LDLIBS) $(SDL_CFLAGS) -lcjson -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL_gfx

t0: $(imgui_target_obj)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(addprefix -I, $(IMGUI_INCLUDE_PATH)) -o bin/$@ src/test/t0/main.cpp $(imgui_target_obj) $(SDL_LDLIBS) -lopengl32

clear:
	@rm -rf $(BUILD_PATH)


update-assets:
	@rm -rf bin/assets
	@mkdir -p bin/assets
	@cp -r D:/OneDrive/resources/SEKIBAKO/* bin/assets/
