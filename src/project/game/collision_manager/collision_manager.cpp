
// collision_box.cpp

#include "collision_manager.h"

#include <algorithm>

CollisionManager* CollisionManager::instance = nullptr;
CollisionManager&
CollisionManager::Instance()
{
    if(instance) return *instance;
    instance = new CollisionManager();
    return *instance;
}

// 创建碰撞盒
CollisionBox*
CollisionManager::Create_collision_box()
{
    CollisionBox* box = new CollisionBox;
    collision_box_list.push_back(box);
    return box;
}

// 销毁碰撞盒
void
CollisionManager::Destroy_collision_box(CollisionBox* collision_box)
{
    // 从列表中移除碰撞盒
    collision_box_list.erase(std::remove(collision_box_list.begin(), collision_box_list.end(), collision_box), collision_box_list.end());

    // 释放碰撞盒内存
    delete collision_box;
}

// 处理碰撞
void
CollisionManager::Process_collide()
{
    // 依次取出所有碰撞箱
    for(CollisionBox* box_src : collision_box_list)
    {
        // 如果未启用碰撞或者检测的碰撞层级为None
        if(!box_src->enable || box_src->layer_dst == CollisionBox::Layer::None) continue;

        // 依次检测所有碰撞箱
        for(CollisionBox* box_dst : collision_box_list)
        {
            // 或者
            // 1.box_dst 碰撞层级和 box_src 不同
            // 2.box_dst 未启用碰撞
            // 3.两个碰撞箱是同一个
            // 则跳过
            if(box_dst->layer_src != box_src->layer_dst || !box_dst->enable || box_src == box_dst) continue;

            // 碰撞检测逻辑
            bool is_collide_x = false;
            bool is_collide_y = false;

            // 计算碰撞盒的边界
            int32_t box_src_left   = box_src->x - (box_src->w / 2);
            int32_t box_src_right  = box_src->x + (box_src->w / 2);
            int32_t box_src_top    = box_src->y - (box_src->h / 2);
            int32_t box_src_bottom = box_src->y + (box_src->h / 2);

            int32_t box_dst_left   = box_dst->x - (box_dst->w / 2);
            int32_t box_dst_right  = box_dst->x + (box_dst->w / 2);
            int32_t box_dst_top    = box_dst->y - (box_dst->h / 2);
            int32_t box_dst_bottom = box_dst->y + (box_dst->h / 2);

            // 检查X轴方向的碰撞
            is_collide_x = (box_src_right >= box_dst_left) && (box_src_left <= box_dst_right);

            // 检查Y轴方向的碰撞
            is_collide_y = (box_src_bottom >= box_dst_top) && (box_src_top <= box_dst_bottom);

            // 如果碰撞
            if(is_collide_x && is_collide_y)
            {
                // 调用碰撞回调函数
                if(box_dst->on_collision) box_dst->on_collision();
            }
        }
    }
}
