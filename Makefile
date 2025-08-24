# ============================================================
# Makefile for cpp-sdl2 + imgui project (MSYS2 mingw64)
# 
# Purpose: Build SDL2 + ImGui C++ project on Windows/MSYS2
# ============================================================

# ---------------------------
# 路径设置
# ---------------------------

MODE ?= debug

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



SAKAEG_PATH := ./src/sakaeg
SAKAEG_OBJ_PATH := $(BUILD_PATH)/sakaeg
SAKAEG_INCLUDE_PATH := $(SAKAEG_PATH)/include
SAKAEG_SRC := $(shell find $(SAKAEG_PATH) -name "*.c" -o -name "*.cpp")

SAKAEG_OBJ := $(SAKAEG_SRC)
SAKAEG_OBJ := $(subst $(SAKAEG_PATH), , $(SAKAEG_OBJ))
SAKAEG_OBJ := $(addprefix $(SAKAEG_OBJ_PATH), $(SAKAEG_OBJ))
SAKAEG_OBJ := $(addsuffix .o, $(SAKAEG_OBJ))


T0_PATH := ./src/test/t0
T0_OBJ_PATH := $(BUILD_PATH)/test/t0
T0_INCLUDE_PATH := $(T0_PATH)

t0_target_src += main.cpp
t0_target_src += header.cpp

t0_target_obj := $(t0_target_src)
t0_target_obj := $(subst $(T0_PATH), , $(t0_target_obj))
t0_target_obj := $(addprefix $(T0_OBJ_PATH)/, $(t0_target_obj))
t0_target_obj := $(addsuffix .o, $(t0_target_obj))


T1_PATH := ./src/test/t1
T1_OBJ_PATH := $(BUILD_PATH)/test/t1
T1_INCLUDE_PATH := $(T1_PATH)

t1_target_src += main.cpp

t1_target_obj := $(t1_target_src)
t1_target_obj := $(subst $(T1_PATH), , $(t1_target_obj))
t1_target_obj := $(addprefix $(T1_OBJ_PATH)/, $(t1_target_obj))
t1_target_obj := $(addsuffix .o, $(t1_target_obj))


T2_PATH := ./src/test/t2
T2_OBJ_PATH := $(BUILD_PATH)/test/t2
T2_INCLUDE_PATH := $(T2_PATH)

t2_target_src += main.cpp
# t2_target_src += stb_image.cpp

t2_target_obj := $(t2_target_src)
t2_target_obj := $(subst $(T2_PATH), , $(t2_target_obj))
t2_target_obj := $(addprefix $(T2_OBJ_PATH)/, $(t2_target_obj))
t2_target_obj := $(addsuffix .o, $(t2_target_obj))


SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDLIBS := $(shell pkg-config --libs sdl2)
ifeq ($(MODE), debug)
	SDL_LDLIBS := $(subst -mwindows, , $(SDL_LDLIBS))
endif

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
	@echo [t0_target_src] $(t0_target_src)
	@echo [t0_target_obj] $(t0_target_obj)
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


$(SAKAEG_OBJ_PATH)/%.cpp.o: $(SAKAEG_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(addprefix -I, $(SAKAEG_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) -c -o $@ $<


$(T0_OBJ_PATH)/%.cpp.o: $(T0_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(addprefix -I, $(IMGUI_INCLUDE_PATH)) -c -o $@ $< $(SDL_CFLAGS)


$(T1_OBJ_PATH)/%.cpp.o: $(T1_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(addprefix -I, $(T1_INCLUDE_PATH) $(SAKAEG_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) -c -o $@ $<


$(T2_OBJ_PATH)/%.cpp.o: $(T2_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(addprefix -I, $(T2_INCLUDE_PATH)) -c -o $@ $<


t0: $(t0_target_obj) $(imgui_target_obj)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) $(addprefix -I, $(IMGUI_INCLUDE_PATH), $(T0_INCLUDE_PATH)) -o bin/$@ $(t0_target_obj) $(imgui_target_obj) $(SDL_LDLIBS) -lopengl32


t1: $(t1_target_obj) $(SAKAEG_OBJ) $(imgui_target_obj)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) \
	$(addprefix -I, $(T1_INCLUDE_PATH) $(SAKAEG_INCLUDE_PATH) $(IMGUI_INCLUDE_PATH)) \
	-o bin/$@ $(t1_target_obj) $(SAKAEG_OBJ) $(imgui_target_obj) \
	$(SDL_LDLIBS) -lopengl32

t2: $(t2_target_obj)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) \
	$(addprefix -I, $(T2_INCLUDE_PATH)) \
	-o bin/$@ $(t2_target_obj) \
	$(SDL_LDLIBS) -lopengl32 -lglew32

clear:
	@rm -rf $(BUILD_PATH)


update-assets:
	@rm -rf bin/assets
	@mkdir -p bin/assets
	@cp -r D:/OneDrive/resources/SEKIBAKO/* bin/assets/
