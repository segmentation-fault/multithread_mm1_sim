#include "mm1_sim/GeneratorAgent.h"

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

namespace mm1_sim {

    std::atomic<std::uint64_t> GeneratorAgent::message_counter_{0};

    GeneratorAgent::GeneratorAgent(IChannelWriter<Message>& output, DurationFunc interval_func, long n_messages)
        : Agent(), n_messages_(n_messages), out_(output), interval_func_(std::move(interval_func)) {}

    void GeneratorAgent::run() {
        while (running_) {
            auto msg = std::make_shared<Message>();
            spdlog::info("[GeneratorAgent] Sending message ID: {}", msg->id);
            out_.push(msg);
            message_counter_.fetch_add(1, std::memory_order_relaxed);
            running_.store(message_counter_.load() < n_messages_);
            std::this_thread::sleep_for(interval_func_());
        }
        auto msg = std::make_shared<Message>(MessageType::Stop);
        out_.push(msg);
    }

} // namespace mm2_sim