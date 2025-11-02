#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
using namespace std;
struct Message {
   int id;
   string content;
};

class MessageQueue {
public:
   MessageQueue() = default;
   void pushMessage(const Message& msg);
   // 从 UI 进程接收消息
   void receiveMessageFromUI(int id, const string& content);
   // 尝试从队列获取一条消息（非阻塞）
   bool tryGetMessage(Message& msg_out);
   bool isEmpty() const;
private:
   // 使用 mutable 关键字允许在 const 成员函数中锁定互斥锁
   mutable mutex m_mutex; 
   queue<Message> m_queue;
};