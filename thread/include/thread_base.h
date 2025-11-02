#pragma once
#include <thread>
#include <atomic>
#include <memory>
#include "common/include/global_context.h"
using namespace std;

class ThreadBase {
public:
    ThreadBase(shared_ptr<GlobalContext> context)
        : m_context(context), m_running(false) {}
    virtual ~ThreadBase() {
        stop();
        join();
    }
    void start() {
        if (m_running) return;
        m_running = true;
        // [C++11 陷阱] 必须使用下面的 lambda 方式来正确捕获 this
        m_thread_obj = thread([this]() { this->run(); });
    }
    void stop() {
        m_running = false;
    }
    void join() {
        if (m_thread_obj.joinable()) {
            m_thread_obj.join();
        }
    }
    bool is_running() const {
        return m_running;
    }
protected:
    virtual void main_loop() = 0;
    shared_ptr<GlobalContext> m_context;
    atomic<bool> m_running;
private:
    void run() {
        while (m_running && m_context->is_running) {
            main_loop();
        }
    }
    thread m_thread_obj;
};