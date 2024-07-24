# CFabric User Guide

CFabric is a simple message pub-sub communication pattern framework for C++17. This guide will help you understand how to use CFabric in your projects.

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Usage](#usage)
   - [Defining Message Types](#defining-message-types)
   - [Creating a Broker](#creating-a-broker)
   - [Subscribing to Messages](#subscribing-to-messages)
   - [Publishing Messages](#publishing-messages)
   - [Creating Subsystems](#creating-subsystems)
4. [Example](#example)
5. [Best Practices](#best-practices)

## Introduction

CFabric provides a flexible and efficient way to implement a publish-subscribe pattern in C++. It allows different parts of your application to communicate without being tightly coupled.

## Installation

CFabric is primarily a header-only library. To use it in your project:

1. Include the `cfabric.hpp` file in your source code.
2. If you're using the provided CMake setup:
   - Add the CFabric directory to your project
   - In your CMakeLists.txt, add `add_subdirectory(path/to/cfabric)`
   - Link your target with CFabric: `target_link_libraries(your_target PRIVATE CFabric)`

## Usage

### Defining Message Types

1. Define your message types in the `BigSystem::MySubsystems::MsgTypes` namespace.
2. Add new types to the `MessageVariants` variant type.

Example:

```cpp
namespace BigSystem::MySubsystems::MsgTypes {
    struct ping {
        std::string source;
        ping(std::string src = "") : source(src) {}
    };

    struct pong {
        std::string source;
        pong(std::string src = "") : source(src) {}
    };

    using MessageVariants = std::variant<ping, pong, /* other message types */>;
}
```

### Creating a Broker

Create a broker instance with your defined message types:

```cpp
auto broker = std::make_shared<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>>();
```

### Subscribing to Messages

Subscribe to messages using either a lambda or a member function:

```cpp
// Using a lambda
broker->subscribe<BigSystem::MySubsystems::MsgTypes::ping>([](const auto& msg) {
    // Handle the ping message
});

// Using a member function
class MySubsystem {
public:
    void handlePing(const BigSystem::MySubsystems::MsgTypes::ping& msg) {
        // Handle the ping message
    }
};

MySubsystem myObject;
broker->subscribe<BigSystem::MySubsystems::MsgTypes::ping>(&myObject, &MySubsystem::handlePing);
```

### Publishing Messages

Publish messages using the broker:

```cpp
BigSystem::MySubsystems::MsgTypes::ping msg("source");
broker->publish(msg);
```

### Creating Subsystems

You can create subsystems that interact with the broker. For example, the `PingPongResponder` class:

```cpp
class PingPongResponder {
public:
    PingPongResponder(std::shared_ptr<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>> broker, std::string name, int max_responses = -1)
        : broker_(broker), name_(name), max_responses_(max_responses) {
        // Subscribe to messages and set up response logic
    }

    void work() {
        // Implement work logic
    }

private:
    std::shared_ptr<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>> broker_;
    std::string name_;
    int max_responses_;
};
```

## Example

Here's a simple example of how to use CFabric:

```cpp
#include "cfabric.hpp"
#include "demo_message_types.hpp"
#include "demo_subsystem.hpp"

int main() {
    auto broker = std::make_shared<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>>();

    BigSystem::MySubsystems::PingPongResponder s1(broker, "s1");
    BigSystem::MySubsystems::PingPongResponder s2(broker, "s2");

    BigSystem::MySubsystems::MsgTypes::ping initial_ping("main");
    broker->publish(initial_ping);

    s1.work();
    s2.work();

    return 0;
}
```

## Best Practices

1. Keep message types simple and preferably trivially copyable.
2. Use meaningful names for message types and their fields.
3. Consider using inheritance for related message types.
4. Unsubscribe from messages when they're no longer needed to prevent memory leaks.
5. Use shared pointers for the broker to ensure proper lifetime management.
6. Design your subsystems to be modular and reusable.
7. Use const references for message parameters in handler functions to avoid unnecessary copying.

For more detailed information and advanced usage, please refer to the comments in the `cfabric.hpp` file and the provided test files.
