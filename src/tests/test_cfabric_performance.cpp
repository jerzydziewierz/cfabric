#include <gtest/gtest.h>
#include "cfabric.hpp"
#include "demo_subsystem.hpp"
#include <chrono>
#include <vector>
#include <spdlog/spdlog.h>

// Inner function that runs a parametrized test
void runPerformanceTest(int num_responses) {
    auto broker = std::make_shared<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>>();

    constexpr int num_subsystems = 2;

    auto s1 = BigSystem::MySubsystems::PingPongResponder(broker, "s1", num_responses);
    auto s2 = BigSystem::MySubsystems::PingPongResponder(broker, "s2", num_responses);

    auto start = std::chrono::high_resolution_clock::now();
    s1.play();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    double elapsed_seconds = elapsed.count();
    double messages_per_second = num_responses / elapsed_seconds;

    SPDLOG_INFO("Performance test with {} responses: {:.1f} messages/sec ({:.1f} k/sec, {:.1f} M/sec)",
                num_responses, messages_per_second, 1e-3*messages_per_second, 1e-6*messages_per_second);

    // Add an assertion to check if the performance is above a certain threshold
    // This threshold can be adjusted based on your performance requirements
    ASSERT_GT(messages_per_second, 1000.0) << "Performance below threshold for " << num_responses << " messages";
}

// Outer function that runs tests with increasing counts of num_responses
TEST(CFabricPerformanceTest, IncreasingResponses) {
    std::vector<int> response_counts = {100, 1000, 10000, 100000, 1000000};

    for (int count : response_counts) {
        SPDLOG_INFO("Starting test with {} responses", count);
        runPerformanceTest(count);
    }
}
