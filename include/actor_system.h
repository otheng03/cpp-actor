#pragma once

#include "message.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

class Actor;

class ActorSystem {
public:
    ActorSystem() = default;
    ~ActorSystem();

    void registerActor(const std::string& name, std::shared_ptr<Actor> actor);
    void unregisterActor(const std::string& name);

    void sendMessage(const std::string& actorName, MessagePtr message);

    void start();
    void stop();

private:
    std::unordered_map<std::string, std::shared_ptr<Actor>> actors_;
    std::mutex actorsMutex_;
    bool running_ = false;
};