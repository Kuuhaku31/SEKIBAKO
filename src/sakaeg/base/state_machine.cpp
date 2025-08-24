
// state_machine.cpp

#include "base.h"

StateNode::StateNode(const std::string& id)
    : state_id(id)
{
}

void
StateNode::On_enter()
{
}

void
StateNode::On_render() const
{
}

void
StateNode::On_update(float)
{
}

void
StateNode::On_update_after(float)
{
}

void
StateNode::On_exit()
{
}

StateMachine::StateMachine()
{
    static StateNode empty_state(STATE_NULL);
    state_pool[empty_state.state_id] = &empty_state;

    current_state = &empty_state;
}

StateMachine::~StateMachine()
{
    Clear_states();
}

void
StateMachine::On_render() const
{
    current_state->On_render();
}
void
StateMachine::On_update(float delta_time)
{
    current_state->On_update(delta_time);
}
void
StateMachine::On_update_after(float delta_time)
{
    current_state->On_update_after(delta_time);
}

// 切换状态
void
StateMachine::Switch_to_state(const char* id)
{
    // 如果存在目标状态，切换状态
    if(state_pool[id])
    {
        current_state->On_exit();
        current_state = state_pool[id];
        current_state->On_enter();
    }
}

// 注册状态
void
StateMachine::Register_state(StateNode* state_node)
{
    state_pool[state_node->state_id] = state_node;
}

void
StateMachine::Clear_states()
{
    // 清空状态机
    for(auto it = state_pool.begin(); it != state_pool.end();)
    {
        if(it->second->state_id == STATE_NULL)
        {
            ++it;
            continue;
        }
        delete it->second;
        it = state_pool.erase(it);
    }
}


std::string
StateMachine::Current_state() const
{
    return current_state->state_id;
}