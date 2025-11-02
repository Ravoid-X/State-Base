#pragma once

#include "state_base.h"
#include "state_begin.h"
#include "state_types.h"
#include "message.h"
#include <memory>
using namespace std;
class StateMachine {
public:
    StateMachine();
    ~StateMachine() = default;
    // 从主循环调用，将消息分发给当前状态
    void handleMessage(const Message& msg);
    void handleError(const string& error);
private:
    void changeState(StateType newState);
    unique_ptr<StateBase> m_currentState;
    StateType m_currentStateType;
};