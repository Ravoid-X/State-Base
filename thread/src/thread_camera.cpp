#include <iostream>
#include "thread_camera.h"
using namespace std;

ThreadCamera::ThreadCamera(shared_ptr<GlobalContext> context)
    : ThreadBase(context) {
}
ThreadCamera::~ThreadCamera() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
}

bool ThreadCamera::initialize_camera(){
    int cam_id = m_context->sys_config.camera_id; 
    int width = m_context->sys_config.camera_width;
    int height = m_context->sys_config.camera_height;
    m_cap.open(cam_id);
    if (!m_cap.isOpened()) {
        cerr << "[ThreadCamera] Error: Failed to open"<<cam_id<<endl;
        return false;
    }
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cout << "[ThreadCamera] Camera initialized successfully." << endl;
    return true;
}

void ThreadCamera::main_loop() {
    cv::Mat local_frame; 
    if (!m_cap.read(local_frame)) { 
        cerr << "[ThreadCamera] Warning: Failed to grab frame." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        return;
    }
    if (local_frame.empty()) {
        return;
    }
    {
        lock_guard<mutex> lock(m_context->image_mutex);
        m_context->shared_image = local_frame.clone();
    }
    this_thread::sleep_for(chrono::milliseconds(33));
}