#include "cfabric.hpp"
#include "demo_message_types.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <atomic>
#include <condition_variable>

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

        class PingPongThreaded {
            using BrokerT = Cfabric::Broker<MsgTypes::MessageVariants>;

        private:
            std::string name;
            std::shared_ptr<BrokerT> broker;
            std::thread thread;
            std::atomic<bool> running{true};
            std::atomic<bool> message_received{false};
            std::condition_variable cv;
            std::mutex mutex;
            int ping_count{0};
            int max_pings{0};

        public:
            PingPongThreaded(std::shared_ptr<BrokerT> broker, std::string name, int max_pings)
                : name(std::move(name)), broker(std::move(broker)), max_pings(max_pings) {
                broker->subscribe<MsgTypes::ping>(this, &PingPongThreaded::on_ping);
                thread = std::thread(&PingPongThreaded::run, this);
            }

            ~PingPongThreaded() {
                running = false;
                cv.notify_one();
                if (thread.joinable()) {
                    thread.join();
                }
            }

            void on_ping(const MsgTypes::ping& msg) {
                std::unique_lock<std::mutex> lock(mutex);
                if (ping_count < max_pings) {
                    SPDLOG_INFO("{} received ping {}", name, ping_count + 1);
                    message_received = true;
                    cv.notify_one();
                }
            }

            void run() {
                while (running && ping_count < max_pings) {
                    std::unique_lock<std::mutex> lock(mutex);
                    cv.wait(lock, [this] { return message_received || !running; });

                    if (!running) break;

                    if (message_received) {
                        ping_count++;
                        if (ping_count < max_pings) {
                            SPDLOG_INFO("{} sending ping {}", name, ping_count);
                            broker->publish(MsgTypes::ping());
                        } else {
                            SPDLOG_INFO("{} finished ping-pong process", name);
                        }
                        message_received = false;
                    }
                }
                running = false;
            }

            void start() {
                SPDLOG_INFO("{} starting ping-pong", name);
                broker->publish(MsgTypes::ping());
            }

            bool is_finished() const {
                return !running;
            }
        };
    } // namespace MySubsystems
} // namespace BigSystem
