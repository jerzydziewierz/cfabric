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
4. [Example](#example)
5. [Best Practices](#best-practices)

## Introduction

CFabric provides a flexible and efficient way to implement a publish-subscribe pattern in C++. It allows different parts of your application to communicate without being tightly coupled.

## Installation

CFabric is a header-only library. To use it in your project, simply include the `cfabric.hpp` file in your source code.

## Usage

### Defining Message Types

1. Define your message types in the `MsgTypes` namespace.
2. Add new types to the `MessageVariants` variant type.
3. Ensure new types are added to the static assert in the subscribe method.

Example:

```cpp
namespace MsgTypes {
    struct MyMessage {
        std::string content;
        MyMessage(std::string content) : content(content) {}
    };

    using MessageVariants = std::variant<MyMessage, /* other message types */>;
}
```

### Creating a Broker

Create a broker instance with your defined message types:

```cpp
auto broker = std::make_shared<Cfabric::Broker<MsgTypes::MessageVariants>>();
```

### Subscribing to Messages

Subscribe to messages using either a lambda or a member function:

```cpp
// Using a lambda
broker->subscribe<MsgTypes::MyMessage>([](const MsgTypes::MyMessage& msg) {
    // Handle the message
});

// Using a member function
class MyClass {
public:
    void handleMessage(const MsgTypes::MyMessage& msg) {
        // Handle the message
    }
};

MyClass myObject;
broker->subscribe<MsgTypes::MyMessage>(&myObject, &MyClass::handleMessage);
```

### Publishing Messages

Publish messages using the broker:

```cpp
MsgTypes::MyMessage msg("Hello, World!");
broker->publish(msg);
```

## Example

Here's a simple example of how to use CFabric:

```cpp
#include "cfabric.hpp"

// Define message types
namespace MsgTypes {
    struct Greeting {
        std::string message;
        Greeting(std::string msg) : message(msg) {}
    };

    using MessageVariants = std::variant<Greeting>;
}

int main() {
    auto broker = std::make_shared<Cfabric::Broker<MsgTypes::MessageVariants>>();

    // Subscribe to Greeting messages
    broker->subscribe<MsgTypes::Greeting>([](const MsgTypes::Greeting& msg) {
        std::cout << "Received greeting: " << msg.message << std::endl;
    });

    // Publish a Greeting message
    broker->publish(MsgTypes::Greeting("Hello, CFabric!"));

    return 0;
}
```

## Best Practices

1. Keep message types simple and preferably trivially copyable.
2. Use meaningful names for message types and their fields.
3. Consider using inheritance for related message types.
4. Unsubscribe from messages when they're no longer needed to prevent memory leaks.
5. Use shared pointers for the broker to ensure proper lifetime management.

For more detailed information and advanced usage, please refer to the comments in the `cfabric.hpp` file.
