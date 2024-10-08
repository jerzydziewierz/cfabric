#include "include/cfabric.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <memory>

// Message definitions
namespace Messages {

    struct StopSignal{};

    struct SourceMessage {
        std::string content;
        SourceMessage(std::string c) : content(std::move(c)) {}
    };

    struct LogOnlyMessage {
        std::string content;
        LogOnlyMessage(std::string c) : content(std::move(c)) {}
    };

    struct ProcessedMessage {
        std::string content;
        ProcessedMessage(std::string c) : content(std::move(c)) {}
    };

    using MessageVariants = std::variant<
            SourceMessage,
            LogOnlyMessage,
            ProcessedMessage,
            StopSignal
            >;
}

// Logger module
class Logger {
public:
    explicit Logger(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) {
        broker->subscribe<Messages::SourceMessage>([this](const Messages::SourceMessage& msg) {
            spdlog::info("Logger: tracing SourceMessage {}", msg.content);
            });
        broker->subscribe<Messages::LogOnlyMessage>([this](const Messages::LogOnlyMessage& msg) {
            spdlog::info("Logger: tracing LogOnlyMessage: {}", msg.content);
        });
        broker->subscribe<Messages::ProcessedMessage>([this](const Messages::ProcessedMessage& msg) {
            spdlog::info("Logger: tracing ProcessedMessage {}", msg.content);
        });
        broker->subscribe<Messages::StopSignal>([this](const Messages::StopSignal& msg) {
            spdlog::info("Logger: Received stop signal");
        });
    }

private:
    // Logger does not need to do anything with the messages
};


// DataProcessor module
class DataProcessor {
public:
    explicit DataProcessor(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) 
        : m_broker(broker) {
        broker->subscribe<Messages::SourceMessage>([this](const Messages::SourceMessage &msg) {
            processAndPublish(msg.content);
        });

        broker->subscribe<Messages::StopSignal>([this](const Messages::StopSignal &msg) {
            spdlog::info("DataProcessor: Received stop signal");
        });
    }

    void processAndPublish(const std::string& data) {
        spdlog::info("DataProcessor: Processing data: {}", data);
        std::string processed_data = "Processed: " + data;
        spdlog::info("DataProcessor: Publishing processed data: {}", processed_data);
        m_broker->publish(Messages::ProcessedMessage(processed_data));
    }

private:
    std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> m_broker;
};

// UserInterface module
class UserInterface {
public:
    explicit UserInterface(std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> broker) 
        : m_broker(broker) {
        broker->subscribe<Messages::StopSignal>([this](const Messages::StopSignal &msg) {
            spdlog::info("UserInterface: Received stop signal");
        });
        broker->subscribe<Messages::SourceMessage>([this](const Messages::SourceMessage &msg) {
            displayMessage(msg);
        });
        broker->subscribe<Messages::ProcessedMessage>([this](const Messages::ProcessedMessage &msg) {
            spdlog::info("UserInterface: Received processed message: {}", msg.content);
        });
    }

    void getUserInput() {
        std::string input = "Simulated user input";
        spdlog::info("Simulating user entering the message : {}", input);
        m_broker->publish(Messages::SourceMessage(input));
    }

private:
    void displayMessage(const Messages::SourceMessage& msg) {
        spdlog::info("UI: Displaying message: {}", msg.content);
    }

    std::shared_ptr<Cfabric::Broker<Messages::MessageVariants>> m_broker;
};

int main() {
    // start the message broker. It is used to tie all the modules together.
    auto broker = std::make_shared<Cfabric::Broker<Messages::MessageVariants>>();

    // Initialize modules
    Logger logger(broker);
    DataProcessor processor(broker);
    UserInterface ui(broker);

    // Simulate application flow
    spdlog::info("Main: Starting the application");
    spdlog::info("Main: starting user-interactive flow");
    ui.getUserInput();
    spdlog::info("Main: starting batch flow");
    // Simulate batch data processing
    processor.processAndPublish("Batch data");
    spdlog::info("Main: stopping the application");
    broker->publish(Messages::StopSignal());

    return 0;
}
