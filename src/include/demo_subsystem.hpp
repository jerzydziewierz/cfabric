#include "cfabric.hpp"
#include <spdlog/spdlog.h>

namespace BigSystem {

    namespace MySubsystems
    {
        class MySubsystem1 {
        private:
            std::string name = "unnamed";
            std::shared_ptr<Cfabric::Broker> broker = nullptr;
            int pong_limit = 0;
        public:
            MySubsystem1(
                    std::shared_ptr<Cfabric::Broker> broker,
                    std::string name = "unset",
                    int pong_limit = 100
                    ) {
                if (!broker) throw std::runtime_error("Broker cannot be null");
                this->broker = broker;

                // subscribe to ping messages
                broker->subscribe(
                        this,
                        &MySubsystem1::on_ping
                        );
            }

            void work() {
                SPDLOG_INFO("{} is doing some work", this->name);
            }

            void play() {
                broker->publish(Cfabric::MsgTypes::ping());
                }

            void on_ping(const Cfabric::MsgTypes::ping& msg) {
                if (pong_limit > 0) broker->publish(Cfabric::MsgTypes::ping());
                pong_limit--;
                }

            void on_string(const Cfabric::MsgTypes::string& msg) {
                SPDLOG_INFO("{} Received string message from {} with value: {}", this->name, msg.source, msg.content);
                }
        };
    } // namespace MySubsystems
} // namespace BigSystem