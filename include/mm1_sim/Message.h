//
// Created by Admin on 2025-05-29.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <chrono>
#include <memory>

namespace mm1_sim {
    enum class MessageType {
        Regular,
        Stop,
        Start
    };

    struct Message {
        using TimePoint = std::chrono::steady_clock::time_point;
        explicit Message(MessageType type = MessageType::Regular);
        virtual ~Message();

        const std::uint64_t id;
        TimePoint creation_time;
        const MessageType type;

        std::string to_string() const;

    private:
        static std::atomic<std::uint64_t> global_id_counter;
    };

    using MessagePtr = std::shared_ptr<Message>;
}
#endif //MESSAGE_H
