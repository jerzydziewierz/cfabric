#include "include/demo_subsystem_1.hpp"
#include <spdlog/spdlog.h>

int main()
{

    // create the broker using the desired message types
    using MyMsgVariants = BigSystem::MySubsystems::MsgTypes::MessageVariants;
    using MyMsgBrokerT = Cfabric::Broker<MyMsgVariants>;
    std::shared_ptr<MyMsgBrokerT> broker = std::make_shared<MyMsgBrokerT>();

    // create the subsystems that will be using the broker -- they can both transmit and receive
    BigSystem::MySubsystems::PingPongResponder s1(broker, "s1");
    BigSystem::MySubsystems::PingPongResponder s2(broker, "s2");
    BigSystem::MySubsystems::PingPongResponder s3(broker, "s3");

    // start the work
    s1.work();
    // after work, play ping-ball
    s1.play();

    return 0;
}