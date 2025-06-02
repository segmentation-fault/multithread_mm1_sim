#ifndef THREADSAFERUNNINGSTATS_H
#define THREADSAFERUNNINGSTATS_H

#include <atomic>
#include <condition_variable>
#include <thread>

namespace mm1_sim {
    // Implements the running weighted mean and variance as described in: D. H. D. West. 1979. Updating mean and variance estimates: an improved method. Commun. ACM 22, 9 (September 1979), 532-535. DOI: https://doi.org/10.1145/359146.359153
    class ThreadSafeRunningStats {
    public:
        ThreadSafeRunningStats();
        void reset();
        void update(double x, double w = 1);
        double mean();
        double std();

    protected:
        std::mutex mutex_;
        std::condition_variable cv_;

        std::atomic<double> M{0};
        std::atomic<double> n{0};
        std::atomic<double> sum_w{0};
        std::atomic<double> T{0};
    };
}

#endif //THREADSAFERUNNINGSTATS_H
