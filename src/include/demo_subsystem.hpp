#include "cfabric.hpp"
#include "demo_message_types.hpp"
#include <spdlog/spdlog.h>

namespace BigSystem {

    namespace MySubsystems
    {
        class PingPongResponder {
            using BrokerT = Cfabric::Broker<MsgTypes::MessageVariants>;

        private:
            std::string name = "unnamed";
            std::shared_ptr<BrokerT> broker = nullptr;
            int response_limit = 0;
        public:

            PingPongResponder(
                    std::shared_ptr<BrokerT> broker,
                    std::string name = "unset",
                    int response_limit = 100
                    ) {
                if (!broker) throw std::runtime_error("Broker cannot be null");
                this->broker = broker;
                this->response_limit = response_limit;

                // subscribe to ping messages
                broker->subscribe(
                        this,
                        &PingPongResponder::on_ping
                        );

                // subscribe to string messages
                broker->subscribe(
                        this,
                        &PingPongResponder::on_string
                        );
            }

            void work() {
                SPDLOG_INFO("{} is doing some work", this->name);
            }

            void play() {
                auto msg = MsgTypes::ping();
                broker->publish(msg);
                }

            void on_ping(const MsgTypes::ping& msg) {
                response_limit--;
                if (response_limit > 0) broker->publish(MsgTypes::ping());

                }

            void on_string(const MsgTypes::string& msg) {
                SPDLOG_INFO("{} Received string message from {} with value: {}", this->name, msg.source, msg.content);
                work();
                }
        };
    } // namespace MySubsystems
} // namespace BigSystem