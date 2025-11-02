#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <chrono>
// third-lib
#include "yaml-cpp/yaml.h"
// common
#include "message.h"
#include "sysconfig.h"
#include "common/include/global_context.h"
// state
#include "state_machine.h" 
// thread
#include "thread/include/thread_camera.h"
#include "thread/include/thread_audio.h"
using namespace std;

int main() {
    // Load Config
    const string config_file_path = "config/config.yaml";
    SysConfig sys_config;
    if (!loadConfiguration(config_file_path, sys_config)){
        return -1;
    }
    // shared context
    auto context = make_shared<GlobalContext>();
    context->is_running = true;
    // Common Initialization
    MessageQueue messageQueue;
    if(context->sys_config.is_use_sql){
        context->db_manager = make_shared<DatabaseManager>();
        if (!context->db_manager->connect(
            context->sys_config.db_host,context->sys_config.db_user,
            context->sys_config.db_pwd,context->sys_config.db_name,
            context->sys_config.db_port
        )){
        cerr<<"[Database] Error: Database connection failed."<<endl;
        return -1;
    }
    }
    // Thread Initialization
    if(context->sys_config.is_use_camera){
        context->camera_thread = make_shared<ThreadCamera>(context);
        context->camera_thread->start();
    }
    if(context->sys_config.is_use_audio){
        context->audio_thread = make_shared<ThreadAudio>(context);
        context->audio_thread->start();
    }
    // State Initialization
    StateMachine stateMachine;
    // Running State Machine
    bool running = true;
    while (running) {
        Message received_msg;
        if (messageQueue.tryGetMessage(received_msg)) {
            if (received_msg.id == -1) {
                running = false;
                continue;
            }
            stateMachine.handleMessage(received_msg);
        } else {
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
    return 0;
}