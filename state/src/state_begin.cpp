#include "state_begin.h"
#include "state_machine.h"
#include <iostream>
using namespace std;

StateBegin::StateBegin(StateMachine* stateMachine)
    : StateBase(stateMachine) {}

void StateBegin::onEnter() {

}

void StateBegin::onExit() {

}

optional<StateType> StateBegin::handleMessage(const Message& msg) {

    return nullopt;
}

void StateBegin::handleError(const string& error) {

}