#pragma once
#include "thread_base.h"

class ThreadAudio : public ThreadBase {
public:
    ThreadAudio(std::shared_ptr<GlobalContext> context);
    virtual ~ThreadAudio();
    bool initialize_Audio();
protected:
    virtual void main_loop() override;
private:
    
};