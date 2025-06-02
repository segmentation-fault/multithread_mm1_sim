#include <iostream>
#include <random>

#include "mm1_sim/Channel.h"
#include "mm1_sim/GeneratorAgent.h"
#include "mm1_sim/Message.h"
#include "mm1_sim/SinkAgent.h"
#include <spdlog/spdlog.h>

#include "mm1_sim/ServerAgent.h"

int main() {
    spdlog::set_level(spdlog::level::debug);

    std::atomic<int> counter{0};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> gen_distr(10.0);
    std::exponential_distribution<> server_distr(10.0);

    auto gen_times = [&gen_distr, &gen]() ->std::chrono::milliseconds {
        return std::chrono::milliseconds(static_cast<int>(gen_distr(gen)));
    };

    auto service_times = [&server_distr, &gen]() ->std::chrono::milliseconds {
        return std::chrono::milliseconds(static_cast<int>(server_distr(gen)));
    };

    mm1_sim::Channel<mm1_sim::Message> generator_to_server;
    mm1_sim::Channel<mm1_sim::Message> server_to_sink;

    mm1_sim::GeneratorAgent generator(generator_to_server, gen_times, 100);
    mm1_sim::ServerAgent server(generator_to_server, server_to_sink, service_times);

    mm1_sim::SinkAgent sink(server_to_sink,
        [&](const mm1_sim::MessagePtr& msg) {
        spdlog::info("[SinkAgent] {} received", msg->to_string());
        counter.fetch_add(1, std::memory_order_relaxed);
    });

    generator.start();
    server.start();
    sink.start();

    generator.join();
    server.join();
    sink.join();

    spdlog::info("{} messages received by the sink", counter.load());
}
