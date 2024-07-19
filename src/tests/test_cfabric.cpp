#include <gtest/gtest.h>
#include "cfabric.hpp"
#include "demo_subsystem.hpp"


TEST(CFabricTest, ExampleTest1) {

    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    Cfabric::MsgTypes::string msg("source", "42");
    BigSystem::MySubsystems::MySubsystem1 s1(broker, "s1");
    BigSystem::MySubsystems::MySubsystem1 s2(broker, "s2");
    BigSystem::MySubsystems::MySubsystem1 s3(broker, "s3");
    s1.work();
}

TEST(CFabricTest, ExampleTest2) {

    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    Cfabric::MsgTypes::ping ball();

    // make 100 subsystems:
    std::vector<BigSystem::MySubsystems::MySubsystem1> subsystems = {};
    for (int i = 0; i < 100; i++) {
        subsystems.push_back(BigSystem::MySubsystems::MySubsystem1(broker, "s" + std::to_string(i)));
        }
    // time playing 100 ping-pong games
    //
    BigSystem::MySubsystems::MySubsystem1 s1(broker, "s1");
    BigSystem::MySubsystems::MySubsystem1 s2(broker, "s2");
    BigSystem::MySubsystems::MySubsystem1 s3(broker, "s3");
    s1.play();
}

// Add more test cases as needed