#pragma once

#include "message.h"
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <functional>

class ActorSystem;

class Actor {
public:
    Actor(const std::string& name, ActorSystem* system);
    virtual ~Actor();

    void send(MessagePtr message);
    void start();
    void stop();

    const std::string& getName() const { return name_; }

protected:
    virtual void onReceive(MessagePtr message) = 0;
    virtual void onStart() {}
    virtual void onStop() {}

    void sendTo(const std::string& actorName, MessagePtr message);

private:
    void processMessages();

    std::string name_;
    ActorSystem* system_;
    std::queue<MessagePtr> messageQueue_;
    std::mutex queueMutex_;
    std::condition_variable queueCondition_;
    std::thread workerThread_;
    std::atomic<bool> running_{false};
};