
// player_states.h

#pragma once

#include "state_machine.h"

class Player;

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
