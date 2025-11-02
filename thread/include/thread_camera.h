#pragma once
#include "thread_base.h"
#include <opencv2/videoio.hpp>

class ThreadCamera : public ThreadBase {
public:
    ThreadCamera(std::shared_ptr<GlobalContext> context);
    virtual ~ThreadCamera();
    bool initialize_camera();
protected:
    virtual void main_loop() override;
private:
    cv::VideoCapture m_cap;
};