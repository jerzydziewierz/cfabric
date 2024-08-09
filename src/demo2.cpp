#include "include/demo_subsystem_2.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <stdexcept>

int main()
{
    // create the broker using the desired message types
    using MyMsgVariants = BigSystem::MySubsystems::MsgTypes::MessageVariants;
    using MyMsgBrokerT = Cfabric::Broker<MyMsgVariants>;
    std::shared_ptr<MyMsgBrokerT> broker;

    try {
        broker = std::make_shared<MyMsgBrokerT>();
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to create broker: {}", e.what());
        return 1;
    }

    const int NUM_MESSAGES = 3;
    auto start_time = std::chrono::high_resolution_clock::now();

    // create a "large processor" system
    SPDLOG_INFO("Creating LargeDataProcessor");
    BigSystem::MySubsystems::LargeDataProcessor s1(broker, "s1");

    for (int i = 1; i <= NUM_MESSAGES; ++i) {
        SPDLOG_INFO("Publishing work {} ...", i);
        broker->publish(BigSystem::MySubsystems::MsgTypes::string("main", "hello " + std::to_string(i)));
    }

    SPDLOG_INFO("Sending stop signal...");
    broker->publish(BigSystem::MySubsystems::MsgTypes::pleaseStop());
    s1.finalize();
    SPDLOG_INFO("s1 returned.");

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    SPDLOG_INFO("Total execution time: {} ms", duration.count());

    return 0;
}
