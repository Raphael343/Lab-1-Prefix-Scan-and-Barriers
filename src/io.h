#ifndef _IO_H
#define _IO_H

#include <argparse.h>
#include <prefix_sum.h>
#include <iostream>
#include <fstream>
#include "pthread_barrier.h"
#include <cstring>

void read_file(struct options_t* args,
               int*              n_vals,
               int*              n_padded_vals,
               int**             input_vals,
               int**             output_vals,
			   int**             temp_vals);

void write_file(struct options_t*         args,
                struct prefix_sum_args_t* opts);

#endif
