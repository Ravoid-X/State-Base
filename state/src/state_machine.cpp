#include "state_machine.h"
#include "state_begin.h"
#include <iostream>
using namespace std;

StateMachine::StateMachine() : m_currentState(nullptr) {
    // 初始状态设置为 Begin
    changeState(StateType::Begin);
}
void StateMachine::handleMessage(const Message& msg) {
    if (m_currentState) {
        auto nextState = m_currentState->handleMessage(msg);
        if (nextState.has_value() && nextState.value() != m_currentStateType) {
            changeState(nextState.value());
        }
    }
}
void StateMachine::handleError(const string& error) {
    if (m_currentState) {
        m_currentState->handleError(error);
    }
}
void StateMachine::changeState(StateType newState) {
    if (m_currentState) {
        m_currentState->onExit();
    }
    //根据新状态类型创建对应的状态对象
    switch (newState) {
        case StateType::Begin:
            m_currentState = make_unique<StateBegin>(this);
            break;
        default:
            cerr<<"[State] Error: "<<newState<<" is not defined"<<endl;
            //m_currentState = nullptr; // 或者切换到一个错误状态
            return;
    }
    m_currentStateType = newState;
    if (m_currentState) {
        m_currentState->onEnter();
    }
}