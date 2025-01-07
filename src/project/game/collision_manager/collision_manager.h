
// collision_manager.h

#pragma once

#include "base.h"

typedef std::function<void()> CollisionCallback;

// 碰撞盒
class CollisionBox : public FRect
{
    friend class CollisionManager;

    // 碰撞层级
    enum class Layer
    {
        None         = 0,
        Player       = 1,
        Enemy        = 2,
        PlayerBullet = 4,
        EnemyBullet  = 8,
        PlayerVfx    = 16,
        EnemyVfx     = 32,
        All          = 0xFFFF,
    };

public:
    bool enable = false; // 是否启用碰撞盒

    CollisionCallback on_collision; // 碰撞回调函数

    Layer layer_src = Layer::None; // 源碰撞层
    Layer layer_dst = Layer::None; // 目标碰撞层

private: // 保证只有碰撞管理器可以创建和销毁碰撞盒
    CollisionBox()  = default;
    ~CollisionBox() = default;
};


typedef std::vector<CollisionBox*> CollisionBoxList;

// 碰撞管理器（单例）
class CollisionManager
{
public:
    static CollisionManager& Instance();

public:
    CollisionBox* Create_collision_box();                   // 创建碰撞盒
    void          Destroy_collision_box(CollisionBox* box); // 销毁碰撞盒
    void          Process_collide();                        // 处理碰撞

private:
    CollisionBoxList collision_box_list; // 碰撞盒列表

private:
    CollisionManager()                                   = default;
    ~CollisionManager()                                  = default;
    CollisionManager(const CollisionManager&)            = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    static CollisionManager* instance;
};
