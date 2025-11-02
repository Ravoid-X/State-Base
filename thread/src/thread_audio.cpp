#include <iostream>
#include "thread/include/thread_audio.h"
using namespace std;

ThreadAudio::ThreadAudio(sys_configshared_ptr<GlobalContext> context)
    : ThreadBase(context) {
}
ThreadAudio::~ThreadAudio() {

}