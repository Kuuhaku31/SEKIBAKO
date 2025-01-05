
// player_states.h

#pragma once

#include "state_machine.h"

class Player;

// 角色状态
#define PLAYER_STATE_IDLE "player_idle"       // 角色空闲
#define PLAYER_STATE_WALK "player_walk"       // 角色行走
#define PLAYER_STATE_RUN "player_run"         // 角色奔跑
#define PLAYER_STATE_JUMP "player_jump"       // 角色跳跃
#define PLAYER_STATE_ASCEND "player_ascend"   // 角色上升
#define PLAYER_STATE_LEVIATE "player_leviate" // 角色悬浮
#define PLAYER_STATE_FALL "player_fall"       // 角色下落
#define PLAYER_STATE_ATTACK "player_attack"   // 角色攻击
#define PLAYER_STATE_HURT "player_hurt"       // 角色受伤
#define PLAYER_STATE_DEAD "player_dead"       // 角色死亡

// 角色空闲
class PlayerStatesIdle : public StateNode
{
public:
    PlayerStatesIdle(Player& player);
    ~PlayerStatesIdle() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色行走
class PlayerStatesWalk : public StateNode
{
public:
    PlayerStatesWalk(Player& player);
    ~PlayerStatesWalk() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色奔跑
class PlayerStatesRun : public StateNode
{
public:
    PlayerStatesRun(Player& player);
    ~PlayerStatesRun() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色跳跃
class PlayerStatesJump : public StateNode
{
public:
    PlayerStatesJump(Player& player);
    ~PlayerStatesJump() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色上升
class PlayerStatesAscend : public StateNode
{
public:
    PlayerStatesAscend(Player& player);
    ~PlayerStatesAscend() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色悬浮
class PlayerStatesLeviate : public StateNode
{
public:
    PlayerStatesLeviate(Player& player);
    ~PlayerStatesLeviate() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};

// 角色下落
class PlayerStatesFall : public StateNode
{
public:
    PlayerStatesFall(Player& player);
    ~PlayerStatesFall() = default;

    void On_enter() override;
    void On_update(float delta_time) override;
    void On_exit() override;

private:
    Player& player;
};
