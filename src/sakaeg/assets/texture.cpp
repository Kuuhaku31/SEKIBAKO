

// texture.cpp

#include "resources_pool.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/gl.h>


// 用 stb_image 加载图片并用 OpenGL 创建纹理，返回纹理ID
uint32_t
load_texture(const char* filename, int* out_width, int* out_height)
{
    // 1. 用 stb_image 加载图片数据，channels=4 强制输出 RGBA 格式
    int      channels = 0;
    uint8_t* data     = stbi_load(filename, out_width, out_height, &channels, 4);
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


void
free_texture(uint32_t* texture_id)
{
    glDeleteTextures(1, texture_id);
    *texture_id = 0;
}


Texture::Texture(const char* filename)
{
    id = load_texture(filename, &size.w, &size.h);
}

Texture::~Texture()
{
    free_texture(&id);
}

uint32_t
Texture::get_id() const
{
    return id;
}

const IRect&
Texture::get_size() const
{
    return size;
}
