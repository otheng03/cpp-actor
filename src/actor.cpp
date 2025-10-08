#include "actor.h"
#include "actor_system.h"
#include <iostream>

Actor::Actor(const std::string& name, ActorSystem* system)
    : name_(name), system_(system) {
}

Actor::~Actor() {
    if (running_) {
        stop();
    }
}

void Actor::send(MessagePtr message) {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        messageQueue_.push(message);
    }
    queueCondition_.notify_one();
}

void Actor::start() {
    if (!running_) {
        running_ = true;
        workerThread_ = std::thread(&Actor::processMessages, this);
        onStart();
    }
}

void Actor::stop() {
    if (running_) {
        running_ = false;
        queueCondition_.notify_all();
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
        onStop();
    }
}

void Actor::sendTo(const std::string& actorName, MessagePtr message) {
    if (system_) {
        system_->sendMessage(actorName, message);
    }
}

void Actor::processMessages() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        queueCondition_.wait(lock, [this] { return !messageQueue_.empty() || !running_; });

        while (!messageQueue_.empty() && running_) {
            auto message = messageQueue_.front();
            messageQueue_.pop();
            lock.unlock();

            try {
                onReceive(message);
            } catch (const std::exception& e) {
                std::cerr << "Actor " << name_ << " error processing message: " << e.what() << std::endl;
            }

            lock.lock();
        }
    }
}