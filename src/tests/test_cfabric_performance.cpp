#include <gtest/gtest.h>
#include "cfabric.hpp"
#include "demo_subsystem_1.hpp"
#include <chrono>
#include <vector>
#include <spdlog/spdlog.h>

TEST(CFabricPerformanceTest, ThreadedPingPong) {
    auto broker = std::make_shared<Cfabric::Broker<BigSystem::MySubsystems::MsgTypes::MessageVariants>>();

    const int num_pings = 1000000;
    auto c1 = std::make_unique<BigSystem::MySubsystems::PingPongThreaded>(broker, "C1", num_pings);
    auto c2 = std::make_unique<BigSystem::MySubsystems::PingPongThreaded>(broker, "C2", num_pings);

    auto start = std::chrono::high_resolution_clock::now();

    c1->start();

    // Wait for both threads to finish
    while (!c1->is_finished() || !c2->is_finished()) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double elapsed_seconds = elapsed.count();
    double pings_per_second = num_pings / elapsed_seconds;

    SPDLOG_INFO("Threaded Ping-Pong Performance: {:.1f} pings/sec", pings_per_second);

    ASSERT_GT(pings_per_second, 100.0) << "Performance below threshold for threaded ping-pong";
    }
