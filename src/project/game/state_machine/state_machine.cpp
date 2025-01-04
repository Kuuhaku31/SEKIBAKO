
// state_machine.cpp

#include "state_machine.h"

StateMachine::~StateMachine()
{
    // 释放所有状态
    for(auto& state : state_pool) delete state.second;
}

void
StateMachine::On_update(float delta)
{
    // 如果当前状态为空，直接返回
    if(!current_state) return;

    // 调用on_update函数
    current_state->On_update(delta);
}

// 切换状态
void
StateMachine::Switch_to(const char* id)
{
    // 如果存在当前状态，退出当前状态
    if(current_state) current_state->On_exit();

    // 如果存在目标状态，进入目标状态
    if(state_pool[id])
    {
        current_state = state_pool[id];
        current_state->On_enter();
    }
}

// 注册状态
void
StateMachine::Register_state(const char* id, StateNode* state_node)
{
    state_pool[id] = state_node;
}
