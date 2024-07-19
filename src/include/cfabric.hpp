#pragma once
#ifndef CFABRIC_HPP
#define CFABRIC_HPP

#include <string>
#include <variant>
#include <functional>
#include <list>
#include <typeindex>
#include <mutex>

#include <spdlog/spdlog.h>

#define CFABRIC_NO_HANDLERS_OK

//! \file cfabric.hpp
//! \brief A simple message pub-sub communication pattern framework for C++17
//! \author George Rey
//!
//! usage:
//! 1. Define message types in MsgTypes namespace - preferably a struct with the message fields. preferably it should be trivially copyable and default constructible
//! 2. Add the new type to the MessageVariants variant type
//! 3. Add the new type to the static assert in the subscribe method
namespace Cfabric {
    //! namespace for message types. Add your message types here.
    namespace MsgTypes {

        //! the message could have no fields, in which case it is a signal. these are useful for simple notifications and are good performance-wise
        struct ping{

            ping() = default;

            //! if the message is not default-copyable, it must have a copy constructor!
            ping(const ping& other) = default;
            };

        //! the message could have fields, in which case it is a data message. these are useful for loose tying of components.
        //! note that a component could publish a message and not care who receives it,
        //! There could be many receivers that react to that message.
        //! There could even be no receivers, and that's fine.
        struct string {
            std::string source;
            std::string content;
            string(std::string source = "unset", std::string content = "unset") : source(source), content(content) {}

            //! if the message is not default-copyable, it must have an implemented explicit copy constructor!
            //! this becomes critically important if your message contains a pointer to something, or other non-trivially copyable data
            string(const string& other) : source(other.source), content(other.content) {}
        };

    //! MessageVariants - must contain all the supported message types to enable the broker to handle them.
    //! When adding a new message type, add it here.
    using MessageVariants = std::variant<ping, string>;

    } // namespace MsgTypes

    class Broker {
        private:
            using HandlerFunction = std::function<void(const MsgTypes::MessageVariants&)>;
            using HandlerList = std::list<HandlerFunction>;
            std::unordered_map<std::type_index, HandlerList> handlers;
            std::mutex mutex;

        public:
            using HandlerID = std::pair<std::type_index, HandlerList::iterator>;


            // subscribe method to be used with directly defined already-capturing lambdas:
            template<typename T>
            HandlerID subscribe(std::function<void(const T&)> handler) {
                //! !! static_assert is used to ensure that the message type is one of the supported types and helps at compile time to catch errors
                //! When adding a new message type, add it here.
                static_assert(
                    std::is_same_v<T, MsgTypes::ping> ||
                    std::is_same_v<T, MsgTypes::string>,
                    "Invalid message type");

                std::lock_guard<std::mutex> guard(mutex);
                auto wrappedHandler = [handler](const MsgTypes::MessageVariants& msg) {
                    if (const T *derived = std::get_if<T>(&msg)) {
                        handler(*derived);
                    }
                };
                auto& handlerList = handlers[std::type_index(typeid(T))];
                auto it = handlerList.insert(handlerList.end(), wrappedHandler);
                return {std::type_index(typeid(T)), it};
        }


    // simplified API that captures the class instance and method of that instance to call:
    template<typename T, typename ClassType>
    HandlerID subscribe(ClassType* instance, void (ClassType::*memberFunction)(const T&)) {
        return subscribe<T>([instance, memberFunction](const T& msg) {
            (instance->*memberFunction)(msg);
        });
    }

    void unsubscribe(const HandlerID& handlerID) {
        std::lock_guard<std::mutex> guard(mutex);
        auto it = handlers.find(handlerID.first);
        if (it != handlers.end()) {
            it->second.erase(handlerID.second);
            }
        }

    void publish(const MsgTypes::MessageVariants& msg) {

        std::visit([this](const auto& concrete_msg) {
            using ConcreteType = std::decay_t<decltype(concrete_msg)>;
            auto it = handlers.find(std::type_index(typeid(ConcreteType)));
            if (it != handlers.end()) {
                // make a copy of the message so that it definetely exists when the handlers are called, even if they are threaded
                // this is a simple way to ensure that the message is not deleted before all handlers have been called
                auto copied_msg = concrete_msg;
                for (const auto& handler : it->second) {
                    handler(MsgTypes::MessageVariants(copied_msg));
                }
                } else {
                // Having no handlers is OK. This means that no one is listening.
                // Alternatively, for debugging or special cases, you might want to log this.
                #ifndef CFABRIC_NO_HANDLERS_OK
                SPDLOG_WARN("No handlers for message type {}", typeid(ConcreteType).name());
                #endif
                }
            }, msg);
        }
    }; // class Broker
} // namespace Cfabric



#endif // CFABRIC_HPP