#include "include/demo_subsystem.hpp"
#include <spdlog/spdlog.h>

int main()
{
    using MyMsgVariants = BigSystem::MySubsystems::MsgTypes::MessageVariants;
    using MyMsgBrokerT = Cfabric::Broker<MyMsgVariants>;
    std::shared_ptr<MyMsgBrokerT> broker = std::make_shared<MyMsgBrokerT>();
    BigSystem::MySubsystems::PingPongResponder s1(broker, "s1");
    BigSystem::MySubsystems::PingPongResponder s2(broker, "s2");
    BigSystem::MySubsystems::PingPongResponder s3(broker, "s3");
    s1.work();
    return 0;
}