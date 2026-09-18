#include "helpers.h"

prefix_sum_args_t* alloc_args(int n_threads) {
  return (prefix_sum_args_t*) malloc(n_threads * sizeof(prefix_sum_args_t));
}

int next_power_of_two(int x) {
    int pow = 1;
    while (pow < x) {
        pow *= 2;
    }
    return pow;
}

void fill_args(prefix_sum_args_t *args,
               int n_threads,
               int n_vals,
               int n_padded_vals,
               int *inputs,
               int *outputs,
               int *temp_vals,
               bool spin,
               int (*op)(int, int, int),
               int n_loops,
               pthread_barrier_t** barrier,
               spin_barrier** custom_barrier) {

    *barrier = nullptr;
    *custom_barrier = nullptr;

    if (spin) {
        *custom_barrier = new spin_barrier(n_threads);
    } else {
        *barrier = (pthread_barrier_t*) malloc(sizeof(pthread_barrier_t));
    }

    for (int i = 0; i < n_threads; ++i) {
        args[i] = {inputs, outputs, temp_vals, spin, n_vals,
                   n_padded_vals, n_threads, i, op, n_loops, *barrier, *custom_barrier};
    }
}