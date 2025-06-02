//
// Created by Admin on 2025-05-29.
//

#ifndef SERVERAGENT_H
#define SERVERAGENT_H

#include "mm1_sim/Agent.h"
#include "mm1_sim/IChannelReader.h"
#include "mm1_sim/Message.h"

#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

#include "IChannelWriter.h"

namespace mm1_sim {

    class ServerAgent final : public Agent {
    public:
        using DurationFunction = std::function<std::chrono::milliseconds()>;

        ServerAgent(IChannelReader<Message>& input_channel,
        IChannelWriter<Message>& output_channel,
                    DurationFunction service_time_func);

    protected:
        void run() override;

    private:

        IChannelReader<Message>& input_channel_;
        IChannelWriter<Message>& output_channel_;
        DurationFunction service_time_func_;
    };

} // namespace mm2_sim

#endif //SERVERAGENT_H
