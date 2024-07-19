#include <gtest/gtest.h>
#include "cfabric.hpp"
#include "demo_subsystem.hpp"


TEST(CFabricTest, ExampleTest1) {

    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    Cfabric::MsgTypes::string msg("source", "42");
    BigSystem::MySubsystems::PingPongResponder s1(broker, "s1");
    BigSystem::MySubsystems::PingPongResponder s2(broker, "s2");
    BigSystem::MySubsystems::PingPongResponder s3(broker, "s3");
    s1.work();
}

TEST(CFabricTest, createPingObject) {

    ASSERT_NO_THROW(
            Cfabric::MsgTypes::ping ball = Cfabric::MsgTypes::ping();
            ) ;
    }

TEST(CFabricTest, ManyPlayers) {

    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    Cfabric::MsgTypes::ping ball = Cfabric::MsgTypes::ping();

    // make many subsystems:
    constexpr int num_subsystems = 10000;
    constexpr int num_responses = 0;

    std::vector<std::shared_ptr<BigSystem::MySubsystems::PingPongResponder>> subsystems = {};

    for (int i = 0; i < num_subsystems; i++) {
        subsystems.push_back(std::make_shared<BigSystem::MySubsystems::PingPongResponder>(broker, "s" + std::to_string(i), num_responses + 1));
        }
    // see the performance of the ping-pong
    // start by checking what time it is using the chrono library
    auto start = std::chrono::high_resolution_clock::now();
    subsystems[0]->play();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double elapsed_seconds = elapsed.count();
    double responses_per_second =  2.0 * num_subsystems / elapsed_seconds;
    SPDLOG_INFO("est. performance, many responders: {} responders: {:.1f} responses/sec ( {:0.1f} k/sec, {:0.1f} M/sec )", num_subsystems, responses_per_second, 1e-3*responses_per_second, 1e-6*responses_per_second);
}

TEST(CFabricTest, ManyResponses) {

    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    Cfabric::MsgTypes::ping ball = Cfabric::MsgTypes::ping();

    // make many subsystems:
    constexpr int num_subsystems = 2;
    constexpr int num_responses = 1000;

    auto s1 = BigSystem::MySubsystems::PingPongResponder(broker, "s1", num_responses);
    auto s2 = BigSystem::MySubsystems::PingPongResponder(broker, "s2", num_responses);
    auto start = std::chrono::high_resolution_clock::now();
    s1.play();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double elapsed_seconds = elapsed.count();
    double passes_per_second = 2.0 * num_responses / elapsed_seconds;
    std::string result = fmt::format("est.performance, 2 responders, {} responses: {:.1f} passes per second ({:.1f} k/sec, {:.1f} M/sec)", num_responses, passes_per_second, 1e-3*passes_per_second, 1e-6*passes_per_second);
    SPDLOG_INFO(result);
}

// Add more test cases as needed