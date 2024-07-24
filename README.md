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
- Simple and lightweight
- you define the data in the messages. CFabric does not impose any restrictions on the message types. You can also have an empty struct as a message type.
- the handler for the message is resolved at compile time from the message type
- the handler is a member function of a class that is subscribed to the message type

## Installation

CFabric is primarily a header-only library. To use it in your project:

1. Include the `cfabric.hpp` file in your source code.
2. If you're using the provided CMake setup:
   - Add the CFabric directory to your project
   - In your CMakeLists.txt, add `add_subdirectory(path/to/cfabric)`
   - Link your target with CFabric: `target_link_libraries(your_target PRIVATE CFabric)`

## Usage

See [/src/demo1](src/demo1.cpp) for a simple example of how to use CFabric.

For a more advanced example of asynchronous data processing using CFabric, check out the `LargeDataProcessor` class in [/src/include/demo_subsystem_2.hpp](src/include/demo_subsystem_2.hpp).

## Best Practices

See [patterns.md](patterns.md) for more information on how to use CFabric in different design patterns.

For more detailed information and advanced usage, please refer to the comments in the `cfabric.hpp` file and the provided test files.
