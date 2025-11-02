#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "sysconfig.h"
using namespace std;

class DatabaseManager;
class ThreadAudio;
class ThreadCamera; 

struct GlobalContext {
    // 线程智能指针
    shared_ptr<ThreadAudio> audio_thread;
    shared_ptr<ThreadCamera> camera_thread;
    // Common 
    shared_ptr<DatabaseManager> db_manager;
    // thread_camera 数据
    cv::Mat shared_image;
    mutex image_mutex;
    // 来自 config.yaml 的配置
    SysConfig sys_config; 
    // 应用程序状态，用于全局停止
    atomic<bool> is_running{true}; 
};