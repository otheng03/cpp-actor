#include "actor_system.h"
#include "actor.h"
#include <iostream>

ActorSystem::~ActorSystem() {
    stop();
}

void ActorSystem::registerActor(const std::string& name, std::shared_ptr<Actor> actor) {
    std::lock_guard<std::mutex> lock(actorsMutex_);
    actors_[name] = actor;
    if (running_) {
        actor->start();
    }
}

void ActorSystem::unregisterActor(const std::string& name) {
    std::lock_guard<std::mutex> lock(actorsMutex_);
    auto it = actors_.find(name);
    if (it != actors_.end()) {
        it->second->stop();
        actors_.erase(it);
    }
}

void ActorSystem::sendMessage(const std::string& actorName, MessagePtr message) {
    std::lock_guard<std::mutex> lock(actorsMutex_);
    auto it = actors_.find(actorName);
    if (it != actors_.end()) {
        it->second->send(message);
    } else {
        std::cerr << "Actor " << actorName << " not found" << std::endl;
    }
}

void ActorSystem::start() {
    running_ = true;
    std::lock_guard<std::mutex> lock(actorsMutex_);
    for (auto& [name, actor] : actors_) {
        actor->start();
    }
}

void ActorSystem::stop() {
    running_ = false;
    std::lock_guard<std::mutex> lock(actorsMutex_);
    for (auto& [name, actor] : actors_) {
        actor->stop();
    }
    actors_.clear();
}