#include <random>

#include "mm1_sim/Channel.h"
#include "mm1_sim/GeneratorAgent.h"
#include "mm1_sim/Message.h"
#include "mm1_sim/SinkAgent.h"
#include <spdlog/spdlog.h>

#include "mm1_sim/ServerAgent.h"
#include "mm1_sim/ThreadSafeRunningStats.h"

int main() {
    spdlog::set_level(spdlog::level::debug);

    std::atomic<int> counter{0};
    mm1_sim::ThreadSafeRunningStats my_stats;

    double gen_rate = 10.0;
    double service_rate = 20.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> gen_distr(gen_rate);
    std::exponential_distribution<> server_distr(service_rate);

    auto gen_times = [&gen_distr, &gen]() ->std::chrono::milliseconds {
        return std::chrono::milliseconds(static_cast<int>(gen_distr(gen)));
    };

    auto service_times = [&server_distr, &gen]() ->std::chrono::milliseconds {
        return std::chrono::milliseconds(static_cast<int>(server_distr(gen)));
    };

    mm1_sim::Channel<mm1_sim::Message> generator_to_server;
    mm1_sim::Channel<mm1_sim::Message> server_to_sink;

    mm1_sim::GeneratorAgent generator(generator_to_server, gen_times, 5000);
    mm1_sim::ServerAgent server(generator_to_server, server_to_sink, service_times);

    mm1_sim::SinkAgent sink(server_to_sink,
        [&](const mm1_sim::MessagePtr& msg) {
            spdlog::info("[SinkAgent] {} received", msg->to_string());
            counter.fetch_add(1, std::memory_order_relaxed);
            auto travel_time =
                std::chrono::steady_clock::now() - msg->creation_time;
            my_stats.update(std::chrono::duration<double>(travel_time).count());
    });

    generator.start();
    server.start();
    sink.start();

    generator.join();
    server.join();
    sink.join();

    spdlog::info("{} messages received by the sink", counter.load());
    spdlog::info("Average system time:  {} s", my_stats.mean());
    spdlog::info("Theoretical average system time:  {} s", 1.0/(service_rate - gen_rate));
}
