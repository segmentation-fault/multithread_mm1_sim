#include "mm1_sim/ThreadSafeRunningStats.h"
# include <cmath>

mm1_sim::ThreadSafeRunningStats::ThreadSafeRunningStats() {}

void mm1_sim::ThreadSafeRunningStats::reset()  {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        this->M.store(0);
        this->n.store(0);
        this->sum_w.store(0);
        this->T.store(0);
    }
    cv_.notify_one();
}

void mm1_sim::ThreadSafeRunningStats::update(double x, double w) {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        double M_val = M.load();
        double sum_w_val = sum_w.load();
        double T_val = T.load();
        double n_val = n.load();

        double Q = x - M_val;
        double TEMP = sum_w_val + w;
        double R = Q * w / TEMP;
        M_val += R;
        T_val += R * sum_w_val * Q;
        sum_w_val = TEMP;
        n_val += 1;

        M.store(M_val);
        T.store(T_val);
        sum_w.store(sum_w_val);
        n.store(n_val);
    }
    cv_.notify_one();
}

double mm1_sim::ThreadSafeRunningStats::mean() {
    std::lock_guard<std::mutex> lock(mutex_);

    return M.load();
}

double mm1_sim::ThreadSafeRunningStats::std() {
    std::lock_guard<std::mutex> lock(mutex_);

    double sum_w_val = sum_w.load();
    double T_val = T.load();
    double n_val = n.load();
    double S2 = 0;

    if (n_val > 1) {
        S2 = T_val * n_val / ((n_val - 1) * sum_w_val);
    }

    return std::sqrt(S2);
}




