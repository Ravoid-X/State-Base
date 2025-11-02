#pragma once
#include "thread_base.h"

class ThreadAudio : public ThreadBase {
public:
    ThreadAudio(shared_ptr<GlobalContext> context);
    virtual ~ThreadAudio();
    bool initialize_audio();
protected:
    virtual void main_loop() override;
private:
    
};