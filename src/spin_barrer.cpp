#include <spin_barrier.h>

spin_barrier::spin_barrier(int n_threads)
    : n_threads_(n_threads)
{
    flags_ = new std::atomic<bool>[n_threads_];

    for (int i = 0; i < n_threads_; i++) {
    flags_[i] = 0;
}
}

spin_barrier::~spin_barrier()
{ 
    delete[] flags_;
}

void spin_barrier::wait(int thread_id)
{
    bool local_go = flags_[thread_id];
    int local_counter = counter_++;

    if (local_counter + 1 == n_threads_) {
        counter_ = 0;

        for (int j = 0; j < n_threads_; ++j) {
            flags_[j] = 1 - flags_[j];
        }
    } else {
        while (flags_[thread_id] == local_go) {
        }
    }
}