#include "include/demo_subsystem.hpp"
#include <spdlog/spdlog.h>

int main()
{
    std::shared_ptr<Cfabric::Broker> broker = std::make_shared<Cfabric::Broker>();
    BigSystem::MySubsystems::MySubsystem1 s1(broker, "s1");
    BigSystem::MySubsystems::MySubsystem1 s2(broker, "s2");
    BigSystem::MySubsystems::MySubsystem1 s3(broker, "s3");
    s1.work();
    return 0;
}