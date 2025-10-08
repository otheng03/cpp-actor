#include "actor.h"
#include "actor_system.h"
#include "message.h"
#include <iostream>
#include <thread>
#include <chrono>

struct PingMessage {
    int count;
    std::string sender;
};

struct PongMessage {
    int count;
    std::string sender;
};

class PingActor : public Actor {
public:
    PingActor(ActorSystem* system) : Actor("ping", system) {}

protected:
    void onStart() override {
        std::cout << "PingActor started" << std::endl;
        auto message = std::make_shared<TypedMessage<PingMessage>>(PingMessage{0, "ping"});
        sendTo("pong", message);
    }

    void onReceive(MessagePtr message) override {
        auto pongMsg = std::dynamic_pointer_cast<TypedMessage<PongMessage>>(message);
        if (pongMsg) {
            const auto& data = pongMsg->getData();
            std::cout << "Ping received: " << data.count << " from " << data.sender << std::endl;

            if (data.count < 5) {
                auto reply = std::make_shared<TypedMessage<PingMessage>>(PingMessage{data.count + 1, "ping"});
                sendTo("pong", reply);
            }
        }
    }
};

class PongActor : public Actor {
public:
    PongActor(ActorSystem* system) : Actor("pong", system) {}

protected:
    void onStart() override {
        std::cout << "PongActor started" << std::endl;
    }

    void onReceive(MessagePtr message) override {
        auto pingMsg = std::dynamic_pointer_cast<TypedMessage<PingMessage>>(message);
        if (pingMsg) {
            const auto& data = pingMsg->getData();
            std::cout << "Pong received: " << data.count << " from " << data.sender << std::endl;

            auto reply = std::make_shared<TypedMessage<PongMessage>>(PongMessage{data.count, "pong"});
            sendTo("ping", reply);
        }
    }
};

int main() {
    ActorSystem system;

    auto pingActor = std::make_shared<PingActor>(&system);
    auto pongActor = std::make_shared<PongActor>(&system);

    system.registerActor("ping", pingActor);
    system.registerActor("pong", pongActor);

    system.start();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    system.stop();

    return 0;
}