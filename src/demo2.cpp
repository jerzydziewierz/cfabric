#include "include/demo_subsystem_2.hpp"
#include <spdlog/spdlog.h>

int main()
{

    // create the broker using the desired message types
    using MyMsgVariants = BigSystem::MySubsystems::MsgTypes::MessageVariants;
    using MyMsgBrokerT = Cfabric::Broker<MyMsgVariants>;
    std::shared_ptr<MyMsgBrokerT> broker = std::make_shared<MyMsgBrokerT>();

    // create a "large processor" system
    SPDLOG_INFO("Creating LargeDataProcessor");
    BigSystem::MySubsystems::LargeDataProcessor s1(broker, "s1");
    SPDLOG_INFO("publishing work 1 ...");
    broker->publish(BigSystem::MySubsystems::MsgTypes::string("main", "hello 1"));
    SPDLOG_INFO("publishing work 2 ...");
    broker->publish(BigSystem::MySubsystems::MsgTypes::string("main", "hello 2"));
    SPDLOG_INFO("publishing work 3 ...");
    broker->publish(BigSystem::MySubsystems::MsgTypes::string("main", "hello 3"));
    SPDLOG_INFO("sending stop signal...");
    broker->publish(BigSystem::MySubsystems::MsgTypes::pleaseStop());
    s1.finalize();
    SPDLOG_INFO("s1 returned.");

    return 0;
}