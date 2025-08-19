# ============================================================
# Makefile for cpp-sdl2 + imgui project (MSYS2 mingw64)
# 
# Purpose: Build SDL2 + ImGui C++ project on Windows/MSYS2
# ============================================================

# ---------------------------
# 路径设置
# ---------------------------
PROJECT_PATH   = ./src/project        # 你的项目源码目录
THIRD_PARTY_DIR = ./src/third_party   # (可选) 第三方目录

PATH_OUTPUT    = ./bin                # 输出目录 (存放 exe)
BUILD_DIR      = ./build              # 对象文件目录 (o 文件)

# ImGui 路径 (请修改为你的实际路径)
IMGUI_DIR = D:/repositories/github/imgui

# 最终输出的可执行文件名
EXE     = main
OUTPUT  = $(PATH_OUTPUT)/$(EXE)

# ---------------------------
# 源文件收集
# ---------------------------
# 项目内所有 cpp
CPP_FILES := $(shell find $(PROJECT_PATH) -name "*.cpp")

# ImGui 源文件
IMGUI_SRC := \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# 总源文件
SOURCES := $(CPP_FILES) $(IMGUI_SRC)

# ---------------------------
# 编译器与参数
# ---------------------------
CXX      = x86_64-w64-mingw32-g++          # mingw64 g++
CXXFLAGS_BASE = -std=c++23 -Wall -Wformat

# include 路径
INCLUDES := \
	$(IMGUI_DIR) \
	$(IMGUI_DIR)/backends

CXXFLAGS_BASE += $(addprefix -I, $(INCLUDES))

# SDL2 flags (从 pkg-config 获取)
SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDLIBS := \
	$(shell pkg-config --libs sdl2) \
	$(shell pkg-config --libs sdl2_image) \
	$(shell pkg-config --libs sdl2_gfx) \
	$(shell pkg-config --libs sdl2_ttf) \
	$(shell pkg-config --libs sdl2_mixer)

LIBS := $(SDL_LDLIBS) -lopengl32

# ---------------------------
# 编译模式 (debug / release)
# ---------------------------
MODE ?= debug

ifeq ($(MODE),debug)
	CXXFLAGS := $(CXXFLAGS_BASE) $(SDL_CFLAGS) -g -O0
	LDFLAGS  :=
else ifeq ($(MODE),release)
	CXXFLAGS := $(CXXFLAGS_BASE) $(SDL_CFLAGS) -O2 -DNDEBUG
	LDFLAGS  := -mwindows
endif

# ---------------------------

# 对象文件路径生成
# 项目源码对象文件（相对路径）
PROJECT_OBJS = $(patsubst ./%, $(BUILD_DIR)/%, $(CPP_FILES:.cpp=.o))
# ImGui 对象文件（绝对路径）
IMGUI_OBJS = $(IMGUI_SRC:.cpp=.o)

OBJS = $(PROJECT_OBJS) $(IMGUI_OBJS)

# ---------------------------
# 规则定义
# ---------------------------

# 默认目标：编译出最终 exe
all: $(OUTPUT)
	@echo "[INFO] Build complete: $(OUTPUT) (MODE=$(MODE))"

# 链接阶段
$(OUTPUT): $(OBJS)
	@mkdir -p $(PATH_OUTPUT)
	$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)


# 项目源码编译规则
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# ImGui 源码编译规则（绝对路径）
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理
clean:
	$(RM) -f $(OUTPUT) $(OBJS)
	$(RM) -rf $(BUILD_DIR)

# 运行（从 bin/ 下启动 exe）
run: $(OUTPUT)
	cd $(PATH_OUTPUT) && ./$(EXE)

# 重新编译后直接运行
crun: clean run
