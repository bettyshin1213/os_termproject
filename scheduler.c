// scheduler.c
#include "utils.h"
#include "scheduler.h"
#include "message_queue.h"

void schedule(int *run_queue, int *run_queue_count, int *wait_queue, int *wait_queue_count, int msgq_id, int *current_process, int output_fd, int current_tick) {
    if (*run_queue_count == 0) return;

    *current_process = run_queue[0];
    printf("Scheduling process %d at tick %d\n", *current_process, current_tick);

    remove_from_queue(run_queue, run_queue_count);

    send_message(*current_process, TIME_QUANTUM, 0, msgq_id);
    printf("Sent time slice to process %d\n", *current_process);
    msgbuf_t msg;
    if (receive_message(&msg, msgq_id) != -1) {
        if (msg.is_io) {
            printf("Process %d moving to wait queue due to I/O request\n", *current_process);
            add_to_queue(*current_process, wait_queue, wait_queue_count);
        } else if (msg.burst_value > 0) {
            printf("Process %d moving back to run queue with remaining burst %d\n", *current_process, msg.burst_value);
            add_to_queue(*current_process, run_queue, run_queue_count);
        }

    }
    printf("Run queue count: %d, Wait queue count: %d\n", *run_queue_count, *wait_queue_count);

    handle_wait_queue(wait_queue, wait_queue_count, run_queue, run_queue_count, msgq_id);
    log_state(output_fd, current_tick, *current_process, msg.burst_value, run_queue, *run_queue_count, wait_queue, *wait_queue_count);
}


void handle_wait_queue(int *wait_queue, int *wait_queue_count, int *run_queue, int *run_queue_count, int msgq_id) {
    for (int i = 0; i < *wait_queue_count; i++) {
        msgbuf_t msg;
        receive_message(&msg, msgq_id);
        if (--msg.burst_value <= 0) {
            add_to_queue(wait_queue[i], run_queue, run_queue_count);
            remove_from_queue(wait_queue, wait_queue_count);
        }
    }
}

void add_to_queue(int pid, int *queue, int *count) {
    queue[(*count)++] = pid;
}

void remove_from_queue(int *queue, int *count) {
    for (int i = 1; i < *count; i++) {
        queue[i - 1] = queue[i];
    }
    (*count)--;
}