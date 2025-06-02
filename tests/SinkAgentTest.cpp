#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "mm1_sim/SinkAgent.h"
#include "mm1_sim/Message.h"
#include "mm1_sim/Channel.h"
#include "mm1_sim/GeneratorAgent.h"

using namespace mm1_sim;

TEST_CASE("generator to sink", "[sink]") {
    std::atomic<int> counter{0};
    Channel<Message> channel;
    GeneratorAgent generator(channel, []() {
        return std::chrono::milliseconds(10);
    }, 10);
    SinkAgent sink(channel, [&](const MessagePtr& msg) {
        std::cout << "[SinkAgent]" << msg->to_string() << " received" << std::endl;
        counter.fetch_add(1, std::memory_order_relaxed);
    });

    generator.start();
    sink.start();

    //std::this_thread::sleep_for(std::chrono::milliseconds(50));

    generator.join();
    sink.join();

    REQUIRE(counter.load() > 0);
}