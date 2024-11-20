// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CHILDREN 10
#define TIME_QUANTUM 8
#define MAX_TIME_TICKS 10000
#define CPU_BURST_MAX 10
#define IO_BURST_MAX 10

typedef struct {
    long mtype;
    int burst_value;  // Can represent CPU or I/O burst
    int is_io;        // 1 if this message indicates an I/O request, 0 for CPU burst
} msgbuf_t;

// Declaration only
void log_state(int output_fd, int tick, int pid, int remaining_burst, int *run_queue, int run_queue_count, int *wait_queue, int wait_queue_count);

#endif