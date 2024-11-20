// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "utils.h"

// Function to schedule processes using the round-robin scheduling algorithm
void schedule(int *run_queue, int *run_queue_count, int *wait_queue, int *wait_queue_count, int msgq_id, int *current_process, int output_fd, int current_tick);

// Function to handle the wait queue, moving processes back to the run queue once I/O burst is complete
void handle_wait_queue(int *wait_queue, int *wait_queue_count, int *run_queue, int *run_queue_count, int msgq_id);

// Helper functions for queue management
void add_to_queue(int pid, int *queue, int *count);
void remove_from_queue(int *queue, int *count);

#endif