#include "include/cfabric.hpp"
#include <iostream>
#include <string>
#include <memory>

// Message definitions
namespace Messages {
    struct SimpleMessage {
        std::string content;
        SimpleMessage(std::string c) : content(std::move(c)) {}
    };

    using MessageVariants = std::variant<SimpleMessage>;
}

// Logger module
class Logger {
public:
    explicit Logger(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) {
        broker->subscribe<Messages::SimpleMessage>([this](const Messages::SimpleMessage& msg) {
            logMessage(msg);
        });
    }

private:
    void logMessage(const Messages::SimpleMessage& msg) {
        std::cout << "Logger: Received message: " << msg.content << std::endl;
    }
};

// DataProcessor module
class DataProcessor {
public:
    explicit DataProcessor(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) 
        : m_broker(broker) {}

    void processAndPublish(const std::string& data) {
        std::string processed_data = "Processed: " + data;
        m_broker->publish(Messages::SimpleMessage(processed_data));
    }

private:
    std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> m_broker;
};

// UserInterface module
class UserInterface {
public:
    explicit UserInterface(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) 
        : m_broker(broker) {
        broker->subscribe<Messages::SimpleMessage>([this](const Messages::SimpleMessage& msg) {
            displayMessage(msg);
        });
    }

    void getUserInput() {
        std::string input;
        std::cout << "Enter a message: ";
        std::getline(std::cin, input);
        m_broker->publish(Messages::SimpleMessage(input));
    }

private:
    void displayMessage(const Messages::SimpleMessage& msg) {
        std::cout << "UI: Displaying message: " << msg.content << std::endl;
    }

    std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> m_broker;
};

int main() {
    auto broker = std::make_shared<Cfabric::Broker<Messages::MessageVariants>>();

    // Initialize modules
    Logger logger(broker);
    DataProcessor processor(broker);
    UserInterface ui(broker);

    // Simulate application flow
    ui.getUserInput();
    processor.processAndPublish("Some data");

    return 0;
}
