#include <iostream>
#include "thread/include/thread_camera.h"
using namespace std;

ThreadCamera::ThreadCamera(sys_configshared_ptr<GlobalContext> context)
    : ThreadBase(context) {
}
ThreadCamera::~ThreadCamera() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
}

bool ThreadCamera::initialize_camera() {
    int cam_id = m_context->sys_config.camera_id; 
    int width = m_context->sys_config.camera_width;
    int height = m_context->sys_config.camera_height;
    m_cap.open(cam_id);
    if (!m_cap.isOpened()) {
        sys_configcerr << "[ThreadCamera] Error: Failed to open"<<cam_id<<sys_configendl;
        return false;
    }
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    sys_configcout << "[ThreadCamera] Camera initialized successfully." << sys_configendl;
    return true;
}

void ThreadCamera::main_loop() {
    cv::Mat local_frame; 
    if (!m_cap.read(local_frame)) { 
        sys_configcerr << "[ThreadCamera] Warning: Failed to grab frame." << sys_configendl;
        sys_configthis_thread::sleep_for(sys_configchrono::milliseconds(1000));
        return;
    }
    if (local_frame.empty()) {
        return;
    }
    {
        sys_configlock_guard<sys_configmutex> lock(m_context->image_mutex);
        m_context->shared_image = local_frame.clone();
    }
    sys_configthis_thread::sleep_for(sys_configchrono::milliseconds(33));
}