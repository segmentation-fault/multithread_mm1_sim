// tests/ChannelTest.cpp
#include <catch2/catch_test_macros.hpp>
#include "mm1_sim/Channel.h"
#include "mm1_sim/IChannelReader.h"
#include "mm1_sim/IChannelWriter.h"
#include "mm1_sim/Message.h"

#include <thread>
#include <chrono>

using namespace mm1_sim;

TEST_CASE("Channel basic push and pop", "[channel]") {
    Channel<Message> ch;

    auto msg = std::make_shared<Message>();
    ch.push(msg);
    auto out = ch.pop();

    REQUIRE(out == msg);
}

TEST_CASE("Channel try_pop returns false on empty", "[channel]") {
    Channel<Message> ch;
    std::shared_ptr<Message> out;

    REQUIRE_FALSE(ch.try_pop(out));
}

TEST_CASE("Channel maintains FIFO order", "[channel]") {
    Channel<Message> ch;

    auto msg1 = std::make_shared<Message>();
    auto msg2 = std::make_shared<Message>();
    ch.push(msg1);
    ch.push(msg2);

    REQUIRE(ch.pop() == msg1);
    REQUIRE(ch.pop() == msg2);
}

TEST_CASE("Channel interfaces work correctly", "[channel][interface]") {
    Channel<Message> ch;

    IChannelWriter<Message>& writer = ch;
    IChannelReader<Message>& reader = ch;

    auto msg = std::make_shared<Message>();
    writer.push(msg);
    auto out = reader.pop();

    REQUIRE(out == msg);
}

TEST_CASE("Consumer blocks on empty queue until producer pushes", "[channel]") {
    Channel<Message> channel;

    bool message_received = false;
    bool consumer_ready = false;

    // Consumer thread: tries to pop immediately (will block)
    std::thread consumer([&] {
        consumer_ready = true;
        auto msg = channel.pop();  // Blocks until producer pushes
        REQUIRE(msg != nullptr);
        REQUIRE(msg->type == MessageType::Regular);
        message_received = true;
    });

    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while (!consumer_ready) {}  // Forcing the producer to start AFTER the consumer

    std::thread producer([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        channel.push(std::make_shared<Message>());
    });

    producer.join();
    consumer.join();

    REQUIRE(message_received);
}

