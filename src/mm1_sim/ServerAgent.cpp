#include "mm1_sim/ServerAgent.h"
#include <iostream>
#include <thread>
#include <spdlog/spdlog.h>

namespace mm1_sim {

    ServerAgent::ServerAgent(IChannelReader<Message>& input_channel,
        IChannelWriter<Message>& output_channel,
                             DurationFunction service_time_func)
        : Agent(), input_channel_(input_channel),
            output_channel_(output_channel),
          service_time_func_(std::move(service_time_func)) {}

    void ServerAgent::run() {
        bool is_stop_received = false;

        while (running_ && !is_stop_received) {
            auto message = input_channel_.pop();
            if (message->type == MessageType::Stop) is_stop_received = true;
            spdlog::info("[ServerAgent] Processing message ID: {}", message->id);
            std::this_thread::sleep_for(service_time_func_());
            spdlog::info("[ServerAgent] Finished processing message ID: {}", message->id);

            spdlog::info("[ServerAgent] [ServerAgent] Sending message ID: {}", message->id);
            output_channel_.push(std::move(message));
        }
        spdlog::info("[ServerAgent] stopped]");
    }
} // namespace mm2_sim
