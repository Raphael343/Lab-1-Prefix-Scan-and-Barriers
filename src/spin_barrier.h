#ifndef SPIN_BARRIER_H
#define SPIN_BARRIER_H

#include <atomic>

class spin_barrier {
public:
    spin_barrier(int n_threads);
    ~spin_barrier();

    void wait(int thread_id);

private:
    int n_threads_;
    std::atomic<int> counter_{0};
    std::atomic<bool>* flags_;
};

#endif