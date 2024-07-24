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
                this->name = name;
                this->response_limit = response_limit;

                // subscribe to play related messages
                broker->subscribe(
                        this,
                        &PingPongResponder::on_ping
                        );

                // subscribe to work related messages
                broker->subscribe(
                        this,
                        &PingPongResponder::on_question
                        );
                broker->subscribe(
                        this,
                        &PingPongResponder::on_answer
                        );
                broker->subscribe(
                        this,
                        &PingPongResponder::on_thanks
                        );

            }

            void work() {
                SPDLOG_INFO("{} is sending the question out", this->name);
                // if I am "s1", I will send a message to "s2"
                if (this->name == "s1") {
                    auto msg = MsgTypes::question ("s1", "what is the answer to life, the universe, and everything?");
                    broker->publish(msg);
                    }
            }

            void on_question(const MsgTypes::question& msg)
            {
                if (this->name != "s1") {
                    SPDLOG_INFO("{} Received question message from {} with value: {}", this->name, msg.source,
                                msg.content);
                    auto response = MsgTypes::answer(this->name, "42");
                    broker->publish(response);
                    }
            }

            void on_answer(const MsgTypes::answer& msg)
            {
                if (this->name == "s1") {
                    SPDLOG_INFO("{} Received answer message from {} with value: {}", this->name, msg.source,
                                msg.content);
                    auto response = MsgTypes::thanks(this->name, "Understood, thanks!");
                    broker->publish(response);
                    }
            }

            void on_thanks(const MsgTypes::thanks& msg)
            {
                if (msg.source != this->name) {
                    SPDLOG_INFO("{} Received thanks message from {} with value: {}", this->name, msg.source,
                                msg.content);
                    }
            }

            void play() {
                for (int i = 0; i < response_limit; ++i) {
                    auto msg = MsgTypes::ping();
                    broker->publish(msg);
                }
            }

            void on_ping(const MsgTypes::ping& msg) {
                // This method is now only used for receiving pings, not for sending them
                // You can add any necessary handling here
            }
        };

        class PingPong2 {
            using BrokerT = Cfabric::Broker<MsgTypes::MessageVariants>;

        private:
            std::string name;
            std::shared_ptr<BrokerT> broker;
            std::atomic<int> pings_sent{0};
            std::atomic<int> pongs_received{0};
            int ping_limit;
            std::atomic<bool> waiting_for_pong{false};

        public:
            PingPong2(std::shared_ptr<BrokerT> broker, std::string name, int ping_limit)
                : name(std::move(name)), broker(std::move(broker)), ping_limit(ping_limit) {
                this->broker->subscribe<MsgTypes::ping>(this, &PingPong2::on_ping);
            }

            void start() {
                if (name == "ping") {
                    send_ping();
                }
            }

            void on_ping(const MsgTypes::ping& msg) {
                if (name == "pong") {
                    broker->publish(MsgTypes::ping());
                } else if (name == "ping") {
                    pongs_received++;
                    waiting_for_pong = false;
                    if (pings_sent < ping_limit) {
                        send_ping();
                    }
                }
            }

            void send_ping() {
                if (!waiting_for_pong) {
                    broker->publish(MsgTypes::ping());
                    pings_sent++;
                    waiting_for_pong = true;
                }
            }

            int get_pings_sent() const {
                return pings_sent;
            }

            int get_pongs_received() const {
                return pongs_received;
            }
        };
    } // namespace MySubsystems
} // namespace BigSystem
