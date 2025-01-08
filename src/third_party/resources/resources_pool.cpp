
// resources_pool.cpp

#include "resources_pool.h"

#include "cJSON.h"

ResourcesPool* ResourcesPool::instance = nullptr;
ResourcesPool&
ResourcesPool::Instance()
{
    if(instance) return *instance;
    instance = new ResourcesPool();
    return *instance;
}

bool
ResourcesPool::LoadResources(Renderer* renderer)
{
    static ImGuiIO& io = ImGui::GetIO();

    static const char resources_path[] = "resources.json";

    cJSON* root = nullptr;

    printf("Load Resources: %s\n", resources_path);

    // 读取资源配置文件
    {
        FILE* file = fopen(resources_path, "r");
        if(!file)
        {
            printf("Error: fopen(): %s\n", strerror(errno));
            return false;
        }

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = (char*)malloc(size + 1);
        if(!buffer)
        {
            printf("Error: malloc(): %s\n", strerror(errno));
            fclose(file);
            return false;
        }

        fread(buffer, 1, size, file);
        buffer[size] = '\0';

        root = cJSON_Parse(buffer);
        if(!root)
        {
            printf("Error: cJSON_Parse(): %s\n", cJSON_GetErrorPtr());
            free(buffer);
            fclose(file);
            return false;
        }

        free(buffer);
        fclose(file);
    }

    { // 加载纹理
        cJSON* texture_info_list = cJSON_GetObjectItem(root, "textures");
        if(cJSON_IsArray(texture_info_list))
        {
            cJSON* item = nullptr;
            cJSON_ArrayForEach(item, texture_info_list)
            {
                const char* label = cJSON_GetObjectItem(item, "label")->valuestring;
                const char* path  = cJSON_GetObjectItem(item, "path")->valuestring;

                texture_pool[label] = IMG_LoadTexture(renderer, path);
            }
        }
    }

    { // 加载字体
        cJSON* font_info_list = cJSON_GetObjectItem(root, "fonts");
        if(cJSON_IsArray(font_info_list))
        {
            cJSON* item = nullptr;
            cJSON_ArrayForEach(item, font_info_list)
            {
                const char* label = cJSON_GetObjectItem(item, "label")->valuestring;
                const char* path  = cJSON_GetObjectItem(item, "path")->valuestring;
                double      size  = cJSON_GetObjectItem(item, "size")->valuedouble;

                font_pool[label] = io.Fonts->AddFontFromFileTTF(path, size, nullptr, io.Fonts->GetGlyphRangesChineseFull());
            }
        }
    }

    { // 加载音效
        cJSON* sound_info_list = cJSON_GetObjectItem(root, "sounds");
        if(cJSON_IsArray(sound_info_list))
        {
            cJSON* item = nullptr;
            cJSON_ArrayForEach(item, sound_info_list)
            {
                const char* label = cJSON_GetObjectItem(item, "label")->valuestring;
                const char* path  = cJSON_GetObjectItem(item, "path")->valuestring;

                sound_pool[label] = Mix_LoadWAV(path);
            }
        }
    }

    { // 加载音乐
        cJSON* music_info_list = cJSON_GetObjectItem(root, "musics");
        if(cJSON_IsArray(music_info_list))
        {
            cJSON* item = nullptr;
            cJSON_ArrayForEach(item, music_info_list)
            {
                const char* label = cJSON_GetObjectItem(item, "label")->valuestring;
                const char* path  = cJSON_GetObjectItem(item, "path")->valuestring;

                music_pool[label] = Mix_LoadMUS(path);
            }
        }
    }

    bool flag = true;

    // 检查加载纹理
    for(const auto& pair : texture_pool)
    {
        if(!pair.second)
        {
            printf("Error: IMG_LoadTexture(): %s\n", pair.first.c_str());
            flag = false;
        }
        else
        {
            printf("Load Texture: %s\n", pair.first.c_str());
        }
    }

    // 检查加载字体
    for(const auto& pair : font_pool)
    {
        if(!pair.second)
        {
            printf("Error: TTF_OpenFont(): %s\n", TTF_GetError());
            flag = false;
        }
        else
        {
            printf("Load Font: %s\n", pair.first.c_str());
        }
    }

    // 检查加载音效
    for(const auto& pair : sound_pool)
    {
        if(!pair.second)
        {
            printf("Error: Mix_LoadWAV(): %s\n", Mix_GetError());
            flag = false;
        }
        else
        {
            printf("Load Sound: %s\n", pair.first.c_str());
        }
    }

    // 检查加载音乐
    for(const auto& pair : music_pool)
    {
        if(!pair.second)
        {
            printf("Error: Mix_LoadMUS(): %s\n", Mix_GetError());
            flag = false;
        }
        else
        {
            printf("Load Music: %s\n", pair.first.c_str());
        }
    }


    return flag;
}

bool
ResourcesPool::FreeResources()
{
    for(auto& pair : texture_pool)
    {
        if(pair.second)
        {
            SDL_DestroyTexture(pair.second);
            pair.second = nullptr;
        }
    }

    for(auto& pair : sound_pool)
    {
        if(pair.second)
        {
            Mix_FreeChunk(pair.second);
            pair.second = nullptr;
        }
    }

    for(auto& pair : music_pool)
    {
        if(pair.second)
        {
            Mix_FreeMusic(pair.second);
            pair.second = nullptr;
        }
    }

    return true;
}
