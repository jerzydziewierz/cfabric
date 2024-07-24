#include "cfabric.hpp"
#include "demo_message_types.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace BigSystem {

    namespace MySubsystems
    {
        class LargeDataProcessor {
            using BrokerT = Cfabric::Broker<MsgTypes::MessageVariants>;

        private:
            std::string name;
            std::shared_ptr<BrokerT> broker;
            std::thread thread;
            std::atomic<bool> running{true};
            std::condition_variable awaiter;
            std::atomic<bool> haveNewData{false};
            std::mutex mutex;

        public:
            LargeDataProcessor(std::shared_ptr<BrokerT> broker, std::string name)
                : name(std::move(name)), broker(broker){
                broker->subscribe<MsgTypes::string>(this, &LargeDataProcessor::on_message);
                thread = std::thread(&LargeDataProcessor::run, this);
            }

            ~LargeDataProcessor() {
                running = false;
                awaiter.notify_one();
                if (thread.joinable()) {
                    thread.join();
                }
            }

            void on_please_stop(const MsgTypes::pleaseStop& msg) {
                running = false;
                awaiter.notify_one();
                }

            void on_message(const MsgTypes::string& msg) {
                // note that this function is executed in the sender's thread, and the broker's code is only a proxy.
                haveNewData = true; // notify the thread to send a response.
                awaiter.notify_one();


                // note that the contents of the message (const MsgTypes::ping& msg) are not guaranteed to be valid after this function returns, so if you need to keep the data, copy it! do not store pointer to the incoming message, that's asking for trouble.
                // note that if the processing **here** is to take a long time, it may be prudent to copy the message/action command into a queue, and do the processing in a thread, to avoid stalling the callers.
                // the caller's "publish" does not finish until this and all other handlers have finished!
                }

            void run() {
                while (running) {
                    std::unique_lock<std::mutex> lock(mutex);
                    awaiter.wait(lock, [this] { return haveNewData || !running; });

                    if (!running) break;

                    if (haveNewData) {
                        haveNewData = false;
                        } else {
                        // if no more data, finish thread.
                        running = false;
                        }
                    }
                }

            void start() {
                }

            bool is_finished() const {
                return !running;
            }
        };
    } // namespace MySubsystems
} // namespace BigSystem
