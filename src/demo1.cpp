#include "include/demo_subsystem.hpp"
#include <spdlog/spdlog.h>

int main()
{
    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    BigSystem::MySubsystems::PingPongResponder s1(broker, "s1");
    BigSystem::MySubsystems::PingPongResponder s2(broker, "s2");
    BigSystem::MySubsystems::PingPongResponder s3(broker, "s3");
    s1.work();
    return 0;
}