
// state_machine.h

#pragma once

#include <string>
#include <unordered_map>

class StateNode
{
public:
    StateNode()  = default;
    ~StateNode() = default;

    virtual void On_enter() {};
    virtual void On_update(float delta_time) {};
    virtual void On_exit() {};
};

typedef std::unordered_map<std::string, StateNode*> StatePool;

class StateMachine
{
public:
    StateMachine() = default;
    ~StateMachine();

    void On_update(float delta_time);

    void Switch_to(const char* id);                             // 切换状态
    void Register_state(const char* id, StateNode* state_node); // 注册状态

private:
    StateNode* current_state = nullptr; // 当前状态
    StatePool  state_pool;              // 状态池
};
