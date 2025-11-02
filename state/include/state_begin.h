#pragma once
#include "state_base.h"
using namespace std;
class StateBegin : public StateBase {
public:
    explicit StateBegin(StateMachine* stateMachine);

    void onEnter() override;
    void onExit() override;
    optional<StateType> handleMessage(const Message& msg) override;
    void handleError(const string& error) override;
};