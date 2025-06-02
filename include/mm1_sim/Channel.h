//
// Created by Admin on 2025-05-29.
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "IChannelReader.h"
#include "IChannelWriter.h"

namespace mm1_sim {

    template<typename T>
    class Channel final : public IChannelReader<T>, public IChannelWriter<T> {
    public:
        void push(std::shared_ptr<T> item) override {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                queue_.push(std::move(item));
            }
            cv_.notify_one();
        }

        std::shared_ptr<T> pop() override {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] () { return !queue_.empty(); });
            auto item = queue_.front();
            queue_.pop();
            return item;
        }

        bool try_pop(std::shared_ptr<T>& out) override {
            std::lock_guard<std::mutex> lock(mutex_);
            if (queue_.empty()) return false;
            out = queue_.front();
            queue_.pop();
            return true;
        }

        std::shared_ptr<T> try_pop_for(std::chrono::milliseconds timeout) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!cv_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
                return nullptr;  // timed out
            }

            auto item = std::move(queue_.front());
            queue_.pop();
            return item;
        }

    private:
        std::queue<std::shared_ptr<T>> queue_;
        std::mutex mutex_;
        std::condition_variable cv_;
    };

} // namespace mm2_sim

#endif //CHANNEL_H
