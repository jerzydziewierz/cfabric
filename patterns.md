# CFabric Design Patterns

This document outlines common design patterns and best practices for using CFabric in your projects.

## 1. Publisher-Subscriber Pattern

CFabric is built around the Publisher-Subscriber pattern. This pattern allows for loose coupling between components in your system.

### Example:

```cpp
auto broker = std::make_shared<Cfabric::Broker<MessageVariants>>();

// Publisher
class Publisher {
public:
    void publishMessage(const std::string& content) {
        broker->publish(MsgTypes::string("Publisher", content));
    }
};

// Subscriber
class Subscriber {
public:
    Subscriber(std::shared_ptr<Cfabric::Broker<MessageVariants>> b) : broker(b) {
        broker->subscribe<MsgTypes::string>(this, &Subscriber::onMessage);
    }

private:
    void onMessage(const MsgTypes::string& msg) {
        std::cout << "Received: " << msg.content << " from " << msg.source << std::endl;
    }
    std::shared_ptr<Cfabric::Broker<MessageVariants>> broker;
};
```

## 2. Command Pattern

CFabric can be used to implement a command pattern, where commands are sent as messages.

### Example:

```cpp
struct Command {
    std::string action;
    std::vector<std::string> parameters;
};

using MessageVariants = std::variant<Command, /* other message types */>;

class CommandExecutor {
public:
    CommandExecutor(std::shared_ptr<Cfabric::Broker<MessageVariants>> b) : broker(b) {
        broker->subscribe<Command>(this, &CommandExecutor::executeCommand);
    }

private:
    void executeCommand(const Command& cmd) {
        // Execute the command based on cmd.action and cmd.parameters
    }
    std::shared_ptr<Cfabric::Broker<MessageVariants>> broker;
};
```

## 3. Event-Driven Architecture

CFabric naturally supports event-driven architectures, where system components react to events (messages) as they occur.

### Example:

```cpp
struct Event {
    std::string type;
    std::string data;
};

using MessageVariants = std::variant<Event, /* other message types */>;

class EventHandler {
public:
    EventHandler(std::shared_ptr<Cfabric::Broker<MessageVariants>> b) : broker(b) {
        broker->subscribe<Event>(this, &EventHandler::handleEvent);
    }

private:
    void handleEvent(const Event& event) {
        // Handle the event based on event.type and event.data
    }
    std::shared_ptr<Cfabric::Broker<MessageVariants>> broker;
};
```

## 4. Asynchronous Processing Pattern

Note that the above examples use synchronous processing : the handler functions are called immediately when a message is published, and the handlers execute in the publisher's thread.
This is not always desirable, especially if the handler functions are time-consuming or block the main thread.

CFabric can be used to implement asynchronous processing patterns, where tasks are processed in the background while the main thread continues execution.
see [src/demo2.cpp](src/demo2.cpp) for an example.



## Best Practices

1. Keep message types simple and preferably trivially copyable.
2. Use const references for message parameters in handler functions.
3. Consider thread safety when designing your message handlers.
4. If the handler functions are time-consuming, consider using asynchronous processing; see [src/demo2.cpp](src/demo2.cpp) for an example. 

By following these patterns and best practices, you can create flexible, maintainable, and efficient systems using CFabric.