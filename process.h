// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include "utils.h"

// Function to simulate a child process executing CPU and I/O bursts
void child_process(int cpu_burst, int io_burst, int msgq_id);

#endif