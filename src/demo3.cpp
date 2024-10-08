#include "include/cfabric.hpp"
#include <iostream>
#include <string>

// Define a simple message type
struct SimpleMessage {
    std::string content;
    SimpleMessage(std::string c) : content(c) {}
};

// Define the message variants
using MessageVariants = std::variant<SimpleMessage>;

int main() {
    // Create a broker
    auto broker = std::make_shared<Cfabric::Broker<MessageVariants>>();

    // Subscribe to SimpleMessage
    broker->subscribe<SimpleMessage>([](const SimpleMessage& msg) {
        std::cout << "Received message: " << msg.content << std::endl;
    });

    // Publish a message
    broker->publish(SimpleMessage("Hello, CFabric!"));

    return 0;
}
