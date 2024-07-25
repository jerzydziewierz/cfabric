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
                // Thread safety: This method is called by the sender's thread(s) and only forwarded here by the broker
                // We use a lock_guard to ensure thread-safe access to the local dataQueue -- which could be used by the local thread
                std::lock_guard<std::mutex> lock(mutex);

                // Memory safety: We're storing a copy of msg.content, not a reference
                // This ensures that we don't have dangling references when the original message is destroyed
                // This is important because the sender's thread(s) may destroy the message right after this method returns
                dataQueue.push(msg.content);

                // Thread synchronization: Notify the local processing thread that new data is available
                // This is safe to call while holding the lock, as notify_one() doesn't block
                awaiter.notify_one();

                // Note: The lock_guard automatically releases the mutex when it goes out of scope
                // This ensures that the mutex is always unlocked, even if an exception is thrown
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
