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

    namespace Static{

        // Helper to check if a type is contained in a variant
        template<typename T, typename Variant>
        struct is_variant_member;

        template<typename T, typename... Ts>
        struct is_variant_member<T, std::variant<Ts...>>
            : std::disjunction<std::is_same<T, Ts>...>
        {};

        // Helper to get the number of types in a variant
        template<typename Variant>
        struct variant_size;

        template<typename... Ts>
        struct variant_size<std::variant<Ts...>>
            : std::integral_constant<std::size_t, sizeof...(Ts)>
        {};

        // The main checking function
        template<typename T, typename Variant>
        constexpr bool check_type_in_variant() {
            if constexpr (is_variant_member<T, Variant>::value) {
                return true;
            } else {
                return false;
            }
        }
    }

    // use template specialization to define the message types
    template<typename MessageVariantsT>
    class Broker {
        private:
            using HandlerFunction = std::function<void(const MessageVariantsT)>;
            using HandlerList = std::list<HandlerFunction>;
            std::unordered_map<std::type_index, HandlerList> handlers;
            std::mutex mutex;

        public:
            using HandlerID = std::pair<std::type_index, typename HandlerList::iterator>;


            // subscribe method to be used with directly defined already-capturing lambdas:
            template<typename T>
            HandlerID subscribe(std::function<void(const T&)> handler) {
            //! static_assert is used to ensure that the message type is one of the supported types and helps at compile time to catch errors
            static_assert(Static::check_type_in_variant<T, MessageVariantsT>(), "T must be one of the types in MsgTypesT variant. Add your specific message type to the variant type that parametrizes the Broker class.");

                std::lock_guard<std::mutex> guard(mutex);
                auto wrappedHandler = [handler](const MessageVariantsT msg) {
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

    void publish(const MessageVariantsT msg) {

        std::visit([this](const auto& concrete_msg) {
            using ConcreteType = std::decay_t<decltype(concrete_msg)>;
            auto it = handlers.find(std::type_index(typeid(ConcreteType)));
            if (it != handlers.end()) {
                // make a copy of the message so that it definetely exists when the handlers are called, even if they are threaded
                // this is a simple way to ensure that the message is not deleted before all handlers have been called
                auto copied_msg = concrete_msg;
                for (const auto& handler : it->second) {
                    handler(MessageVariantsT(copied_msg));
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