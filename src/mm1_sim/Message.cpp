#include "mm1_sim/Message.h"

#include <atomic>
#include <sstream>

std::atomic<std::uint64_t> mm1_sim::Message::global_id_counter{0};

mm1_sim::Message::Message(MessageType type)
    : id(global_id_counter.fetch_add(1, std::memory_order_relaxed)),
creation_time(std::chrono::steady_clock::now()), type(type) {}

mm1_sim::Message::~Message() = default;

std::string mm1_sim::Message::to_string() const {
    using namespace std::chrono;
    auto ms = duration_cast<milliseconds>(creation_time.time_since_epoch()).count();

    std::ostringstream oss;
    oss << "Message{id=" << id << ", type="
       << (type == MessageType::Stop ? "Stop" : "Regular")
       << ", time=" << ms << " ms}";
    return oss.str();
}
