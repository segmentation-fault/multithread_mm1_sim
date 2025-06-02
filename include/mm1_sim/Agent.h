//
// Created by Admin on 2025-05-29.
//

#ifndef AGENT_H
#define AGENT_H
#include <atomic>
#include <thread>

#include "Deprecated.h"
#include "IChannelReader.h"
#include "Message.h"

namespace mm1_sim {

    class Agent {
    public:
        explicit Agent()
             = default;

        virtual ~Agent() {
            stop();
        }

        void start() {
            running_ = true;
            thread_ = std::thread(&Agent::run, this);
        }

        // DEPRECATED("Use join() instead; stop may lead to a deadlock")
        void stop() {
            running_ = false;
            if (thread_.joinable()) thread_.join();
        }

        void join() {
            thread_.join();
        }

        virtual void run() = 0;

    protected:
        std::atomic<bool> running_{false};
        std::thread thread_;
    };

} // namespace mm2_sim

#endif //AGENT_H
