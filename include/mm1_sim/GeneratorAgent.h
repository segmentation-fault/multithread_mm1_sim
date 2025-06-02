//
// Created by Admin on 2025-05-29.
//

#ifndef GENERATORAGENT_H
#define GENERATORAGENT_H

#include "Agent.h"
#include "Message.h"
#include "IChannelWriter.h"

#include <functional>
#include <chrono>
#include <memory>
#include <climits>

namespace mm1_sim {

    class GeneratorAgent final : public Agent {
    public:
        using DurationFunc = std::function<std::chrono::milliseconds()>;

        GeneratorAgent(IChannelWriter<Message>& output, DurationFunc interval_func, long n_messages = LONG_MAX);

    protected:
        void run() override;

    private:
        long n_messages_;
        IChannelWriter<Message>& out_;
        DurationFunc interval_func_;
        static std::atomic<std::uint64_t> message_counter_;
    };

} // namespace mm2_sim

#endif //GENERATORAGENT_H
