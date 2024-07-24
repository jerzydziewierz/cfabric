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

## Best Practices

1. Keep message types simple and preferably trivially copyable.
2. Use const references for message parameters in handler functions.
3. Unsubscribe handlers when they are no longer needed.
4. Use shared pointers for the broker to ensure proper lifetime management.
5. Design your subsystems to be modular and reusable.
6. Consider thread safety when designing your message handlers.

By following these patterns and best practices, you can create flexible, maintainable, and efficient systems using CFabric.

## 4. Asynchronous Processing Pattern

CFabric can be used to implement asynchronous processing patterns, where tasks are processed in the background while the main thread continues execution.

### Example:

```cpp
class AsyncProcessor {
public:
    AsyncProcessor(std::shared_ptr<Cfabric::Broker<MessageVariants>> b) : broker(b) {
        broker->subscribe<TaskMessage>(this, &AsyncProcessor::processTask);
        broker->subscribe<StopMessage>(this, &AsyncProcessor::stop);
        processingThread = std::thread(&AsyncProcessor::run, this);
    }

    ~AsyncProcessor() {
        if (processingThread.joinable()) {
            processingThread.join();
        }
    }

private:
    void processTask(const TaskMessage& task) {
        // Process the task asynchronously
    }

    void stop(const StopMessage& msg) {
        running = false;
    }

    void run() {
        while (running) {
            // Process messages from the broker
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::shared_ptr<Cfabric::Broker<MessageVariants>> broker;
    std::thread processingThread;
    std::atomic<bool> running{true};
};
```

This pattern allows for efficient handling of time-consuming tasks without blocking the main execution thread. It's particularly useful for scenarios where you need to process large amounts of data or perform long-running operations while keeping your application responsive.
