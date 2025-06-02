//
// Created by Admin on 2025-05-29.
//

#ifndef SINKAGENT_H
#define SINKAGENT_H

#include "Agent.h"
#include "Message.h"
#include "IChannelReader.h"

#include <functional>

#include "spdlog/spdlog.h"

namespace mm1_sim {

    class SinkAgent final : public Agent {
    public:
        using Callback = std::function<void(MessagePtr)>;

        SinkAgent(IChannelReader<Message>& input_channel,
                  Callback on_receive)
            : Agent(),
              input_channel_(input_channel),
        callback_(std::move(on_receive)) {}

    protected:
        void run() override {
            while (running_) {
                auto message = input_channel_.pop();
                if (message->type == MessageType::Stop) break;
                callback_(std::move(message));
            }
            spdlog::info("[SinkAgent] stopped]");
        }

    private:

        IChannelReader<Message>& input_channel_;
        Callback callback_;
    };

} // namespace mm2_sim

#endif //SINKAGENT_H
