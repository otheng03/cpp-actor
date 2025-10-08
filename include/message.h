#pragma once

#include <memory>
#include <string>
#include <typeinfo>

class Message {
public:
    virtual ~Message() = default;
    virtual std::string getType() const = 0;
};

template<typename T>
class TypedMessage : public Message {
public:
    TypedMessage(T data) : data_(std::move(data)) {}

    const T& getData() const { return data_; }

    std::string getType() const override {
        return typeid(T).name();
    }

private:
    T data_;
};

using MessagePtr = std::shared_ptr<Message>;