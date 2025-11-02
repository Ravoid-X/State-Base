#include "message.h"
#include <chrono>
void MessageQueue::pushMessage(const Message& msg) {
   unique_lock<mutex> lock(m_mutex);
   m_queue.push(msg);
}

void MessageQueue::receiveMessageFromUI(int id, const string& content) {
   Message msg = {id, content};
   unique_lock<mutex> lock(m_mutex);
   m_queue.push(msg);
}

bool MessageQueue::tryGetMessage(Message& msg_out) {
   unique_lock<mutex> lock(m_mutex);
   if (m_queue.empty()) {
      return false;
   }
   msg_out = m_queue.front();
   m_queue.pop();
   return true;
}

bool MessageQueue::isEmpty() const {
   unique_lock<mutex> lock(m_mutex);
   return m_queue.empty();
}