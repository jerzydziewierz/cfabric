#pragma once
#ifndef CFABRICDEMO1_MESSAGE_TYPES_HPP
#define CFABRICDEMO1_MESSAGE_TYPES_HPP

namespace BigSystem {

    namespace MySubsystems {
        //! namespace for message types. Add your message types here.
        namespace MsgTypes {

            //! the message could have no fields, in which case it is a signal. these are useful for simple notifications and are good performance-wise
            struct ping {

                ping() = default;

                //! if the message is not default-copyable, it must have a copy constructor!
                ping(const ping &other) = default;
            };

            //! signal to stop the threads.
            struct pleaseStop{};

            struct pingTTL {
            private:

                int _ttl = 10;
            public:
                int source = 0;
                int destination = 0;

                pingTTL(int ttl = 10) : _ttl(ttl) {}

                pingTTL(
                        int ttl = 10,
                        int source = 0,
                        int destination = 0) :
                        _ttl(ttl),
                        source(source),
                        destination(destination) {};

                //! if the message is not default-copyable, it must have a copy constructor!
                pingTTL(const pingTTL &other) = default;

            };

            //! the message could have fields, in which case it is a data message. these are useful for loose tying of components.
            //! note that a component could publish a message and not care who receives it,
            //! There could be many receivers that react to that message.
            //! There could even be no receivers, and that's fine.
            struct string {
                std::string source;
                std::string content;

                string(
                        std::string source = "unset",
                        std::string content = "unset"
                        ) :
                        source(source),
                        content(content)
                        {}

                //! if the message is not default-copyable, it must have an implemented explicit copy constructor!
                //! this becomes critically important if your message contains a pointer to something, or other non-trivially copyable data
                string(const string &other) : source(other.source), content(other.content){}
            };

            // message types can and should derive from simpler messages where applicable. Use that instead of repeating the same fields, or enums. This is a rust like good practice.

            struct question: string {
                question(std::string source = "unset", std::string content = "unset") : string(source, content) {}
            };
            struct answer: string {
                answer(std::string source = "unset", std::string content = "unset") : string(source, content) {}
            };
            struct thanks: string {
                thanks(std::string source = "unset", std::string content = "unset") : string(source, content) {}
            };


            //! MessageVariants - must contain all the supported message types to enable the broker to handle them.
            //! When adding a new message type, add it here.
            using MessageVariants = std::variant<ping, pingTTL, question, answer, thanks, string, pleaseStop>;

        } // namespace MsgTypes
    } // namespace Cfabric
}
#endif //CFABRICDEMO1_MESSAGE_TYPES_HPP