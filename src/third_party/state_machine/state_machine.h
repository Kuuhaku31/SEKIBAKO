
// state_machine.h

#pragma once

#include <string>
#include <unordered_map>

#define STATE_NULL "state_null"

class StateNode
{
public:
    StateNode(const char* id);
    ~StateNode() = default;

    virtual void On_enter() {}
    virtual void On_render() const {}
    virtual void On_update(float delta_time) {}
    virtual void On_update_after(float delta_time) {}
    virtual void On_exit() {}

    const char* const state_id;
};

typedef std::unordered_map<std::string, StateNode*> StatePool;

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void On_render() const { current_state->On_render(); }
    void On_update(float delta_time) { current_state->On_update(delta_time); }
    void On_update_after(float delta_time) { current_state->On_update_after(delta_time); }

    void Switch_to_state(const char* id);       // 切换状态
    void Register_state(StateNode* state_node); // 注册状态
    void Clear_states();                        // 清空状态机

    const char* Current_state() const { return current_state->state_id; }

private:
    StateNode* current_state = nullptr; // 当前状态
    StatePool  state_pool;              // 状态池
};
