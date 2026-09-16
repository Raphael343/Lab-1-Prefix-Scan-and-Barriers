#include "prefix_sum.h"
#include "helpers.h"
#include "pthread_barrier.h"

void* compute_prefix_sum(void *a)
{
    prefix_sum_args_t *args = (prefix_sum_args_t *)a;

    // Up Sweep
    int group_size = 1;
    while (group_size < args->n_padded_vals) {
        group_size *= 2;
        int num_tasks = args->n_padded_vals / group_size;
        // Process the level
        for(int task_num = args->t_id; task_num < num_tasks; task_num += args->n_threads){
            int group_start = task_num * group_size;
            int left_index  = group_start + group_size / 2 - 1;
            int right_index = group_start + group_size - 1;

            args->temp_vals[right_index] = args->op(args->temp_vals[left_index], args->temp_vals[right_index], args->n_loops);
        }

        // Barrier
        pthread_barrier_wait(args->barrier);
    }

    if (args->t_id == 0){
        args->temp_vals[args->n_padded_vals - 1] = 0;
    }

    pthread_barrier_wait(args->barrier);

    // Downsweep
    while (group_size >= 2) {
        int num_tasks = args->n_padded_vals / group_size;
        // Process the level
        for(int task_num = args->t_id; task_num < num_tasks; task_num += args->n_threads){
            int group_start = task_num * group_size;
            int left_index  = group_start + group_size / 2 - 1;
            int right_index = group_start + group_size - 1;
            
            int temp_left = args->temp_vals[left_index];
            int temp_right = args->temp_vals[right_index];

            args->temp_vals[left_index] = temp_right;
            args->temp_vals[right_index] = args->op(temp_right, temp_left, args->n_loops);
        }
        
        group_size /= 2;
        // Barrier
        pthread_barrier_wait(args->barrier);
    }

    // Inclusive add
    int num_tasks = args->n_vals;
    for(int task_num = args->t_id; task_num < num_tasks; task_num += args->n_threads){
        args->output_vals[task_num] = args->op(args->temp_vals[task_num], args->input_vals[task_num], args->n_loops);
    }
    
    return 0;
}
// lldb ./bin/prefix_scan
// process launch -- -n 1 -i tests/seq_64_test.txt -o test.txt -l 10
