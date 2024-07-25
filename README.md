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

Key features of CFabric include:
- broker based pub-sub communication pattern
- in-memory only operation, no network
    - if you need your messages to go over network or to a different process, you are probably thinking about [mosquitto](https://mosquitto.org/documentation/) and [https://github.com/eclipse/paho.mqtt.cpp](https://github.com/eclipse/paho.mqtt.cpp)
- you define the data in the messages. CFabric does not impose any restrictions on the message types. You can also have an empty struct as a message type.
- the handler for the message is resolved at compile time from the message type

## What CFabric doesn't do natively

- Asynchronous operation. By default all interactions are eager and synchronous for simplicity - "deep-first".
    - You can extend the operation to be asynchronous as per example in `src/demo2.cpp`
- Test for message loops. If the message handler causes the sender to create a message that will cause an infinite loop, or even a chain that is too long, then the program will segfault on stack overflow.
   - You can prevent that by adding "Time To Live" (TTL) field in the source message, and use that to limit recursion.

## Installation

CFabric is a header-only library. To use it in your project:

1. Include the `cfabric.hpp` file in your source code.
2. Create your message types struct, and a special variadic type that contains all the message types for given broker
3. Instantiate the broker under std::shared_ptr<> pointer
4. Pass that pointer to all the clients that will listen to messages
5. the clients subscribe to message types
6. run the program, and profit!


If you're using the provided CMake setup:
   - Add the CFabric directory to your project
   - In your CMakeLists.txt, add `add_subdirectory(path/to/cfabric)`
   - Link your target with CFabric: `target_link_libraries(your_target PRIVATE CFabric)`

## Usage

See [/src/demo1](src/demo1.cpp) for a simple example of how to use CFabric.

For a more advanced example of asynchronous data processing using CFabric, check out the `LargeDataProcessor` class in [/src/include/demo_subsystem_2.hpp](src/include/demo_subsystem_2.hpp).

## Best Practices

See [patterns.md](patterns.md) for more information on how to use CFabric in different design patterns.

For more detailed information and advanced usage, please refer to the comments in the `cfabric.hpp` file and the provided test files.
