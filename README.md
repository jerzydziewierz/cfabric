![CI](https://github.com/jerzydziewierz/cfabric/actions/workflows/ci.yml/badge.svg)

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

## Advantages of the pub-sub communication pattern

As compared to traditional function calling, event-driven programming, callback driven, worker pool driven, or other communication patterns, the pub-sub pattern has several advantages:

* Loose coupling between components, and very clear separation of concerns
    * It's easy to sell work on each component to the customer
    * It's easy to onboard new developers, and split work between them
    * It's easy to replace one component with another, or disable parts of the system, or add new components as needed for specific purposes 
* Testability: 
    * It's easy to test individual components in isolation. 
    * The message types are very clearly defined, so you can easily write unit tests for each message type.
    * it's easy to write integration tests for the whole system by sending messages to the broker and checking the results.
* Maintainability:  
    * It's easy to draw a diagram of the message flow in your system. 
    * It's easy to intercept and log messages for debugging purposes.
    * When something needs to change, it's easy to see what parts of the system will be affected, and isolate the changes to those parts.

## Introduction

CFabric provides a flexible and efficient way to implement a publish-subscribe pattern in C++. It allows different parts of your application to communicate without being tightly coupled.

Key features of CFabric include:
- Broker-based pub-sub communication pattern
- In-memory-only operation, no network
    - if you need your messages to go over network or to a different process, you are probably thinking about [mosquitto](https://mosquitto.org/documentation/) and [https://github.com/eclipse/paho.mqtt.cpp](https://github.com/eclipse/paho.mqtt.cpp)
- The handler for the message is resolved at compile time from the message type 
- focus on memory safe operation, with no memory leaks or undefined behaviour
- No external dependencies, simple to integrate
- Simple and easy to extend or customize with e.g. message filtering, asynchronous operation, work queues, etc.
- Useful error messages for common problems
- No restrictions on the message data types. You can also have an empty struct as a message type, which is useful for signaling notifications.
- We Can have multiple brokers in the same program, each with its own set of message types, to further separate concerns

## What CFabric doesn't do natively

- Test for message loops. If the message handler causes the sender to create a message that will cause an infinite loop, or even a chain that is too long, then the program will segfault on stack overflow.
   - You can prevent that by adding "Time To Live" (TTL) field in the source message, and use that to limit recursion.
- Asynchronous operation. By default, all interactions are eager and synchronous - for simplicity - "deep-first".
    - You can extend the operation to be asynchronous as per example in `src/demo2.cpp`
    - You can also use a worker pool to concurrently process messages in parallel, or use a message queue to buffer messages.

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
