#include <iostream>
#include "thread_audio.h"
using namespace std;

ThreadAudio::ThreadAudio(shared_ptr<GlobalContext> context)
    : ThreadBase(context) {
}
ThreadAudio::~ThreadAudio() {

}
bool ThreadAudio::initialize_audio() {

    return true;
}
void ThreadAudio::main_loop() {

}