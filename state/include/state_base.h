#pragma once

#include "message.h"
#include "state_types.h"
#include <string>
#include <optional> // 用于返回可能的状态切换请求
using namespace std;

class StateMachine;

class StateBase {
public:
    explicit StateBase(StateMachine* stateMachine);
    virtual ~StateBase() = default;
    StateBase(const StateBase&) = delete;
    StateBase& operator=(const StateBase&) = delete;
    // 进入该状态时执行的操作
    virtual void onEnter() = 0;
    // 退出该状态时执行的操作
    virtual void onExit() = 0;
    // 处理从消息队列传来的消息
    virtual optional<StateType> handleMessage(const Message& msg) = 0;
    // 处理系统级错误
    virtual void handleError(const std::string& error) = 0;
protected:
    StateMachine* m_stateMachine;
};