#include "cfabric.hpp"
#include "demo_message_types.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

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
            std::mutex mutex;
            std::queue<std::string> dataQueue;

        public:
            LargeDataProcessor(std::shared_ptr<BrokerT> broker, std::string name)
                : name(std::move(name)), broker(broker){
                broker->subscribe<MsgTypes::string>(this, &LargeDataProcessor::on_message);
                broker->subscribe<MsgTypes::pleaseStop>(this, &LargeDataProcessor::on_please_stop);
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
                std::lock_guard<std::mutex> lock(mutex);
                dataQueue.push(msg.content);
                awaiter.notify_one();
            }

            void run() {
                while (running) {
                    std::unique_lock<std::mutex> lock(mutex);
                    awaiter.wait(lock, [this] { return !dataQueue.empty() || !running; });

                    if (!running) break;

                    while (!dataQueue.empty()) {
                        std::string data = dataQueue.front();
                        dataQueue.pop();
                        lock.unlock();

                        // Process the data here
                        spdlog::info("Processing data: {}", data);
                        // simulate that it takes 1 second to process this data:
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        spdlog::info("done.");
                        lock.lock();
                    }
                }
            }

            void finalize() {
                if (thread.joinable()) {
                    thread.join();
                }
            }

            void start() {
                // This method is now empty as the thread starts in the constructor
            }

            bool is_finished() const {
                return !running;
            }
        };
    } // namespace MySubsystems
} // namespace BigSystem
