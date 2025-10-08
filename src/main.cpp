#include "actor.h"
#include "actor_system.h"
#include "message.h"
#include <iostream>
#include <thread>
#include <chrono>

struct HelloMessage {
    std::string text;
};

class HelloActor : public Actor {
public:
    HelloActor(ActorSystem* system) : Actor("hello", system) {}

protected:
    void onStart() override {
        std::cout << "HelloActor started" << std::endl;
    }

    void onReceive(MessagePtr message) override {
        auto helloMsg = std::dynamic_pointer_cast<TypedMessage<HelloMessage>>(message);
        if (helloMsg) {
            std::cout << "HelloActor received: " << helloMsg->getData().text << std::endl;
        }
    }
};

int main() {
    std::cout << "Starting C++ Actor System" << std::endl;

    ActorSystem system;
    auto helloActor = std::make_shared<HelloActor>(&system);

    system.registerActor("hello", helloActor);
    system.start();

    auto message = std::make_shared<TypedMessage<HelloMessage>>(HelloMessage{"Hello, Actor World!"});
    system.sendMessage("hello", message);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    system.stop();

    std::cout << "Actor system stopped" << std::endl;

    return 0;
}