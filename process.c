// process.c
#include "utils.h"
#include "message_queue.h"

void child_process(int cpu_burst, int io_burst, int msgq_id) {
    printf("Child process %d started with CPU burst %d and I/O burst %d\n", getpid(), cpu_burst, io_burst);

    while (1) {
        msgbuf_t msg;
        if (receive_message(&msg, msgq_id) != -1) {
            printf("Process %d received time slice with burst %d\n", getpid(), msg.burst_value);

            if (msg.burst_value >= cpu_burst) {
                cpu_burst = 0;
                printf("Process %d completed CPU burst, moving to I/O with burst %d\n", getpid(), io_burst);
                send_message(getpid(), io_burst, 1, msgq_id);
            } else {
                cpu_burst -= msg.burst_value;
                printf("Process %d has remaining CPU burst %d\n", getpid(), cpu_burst);
                send_message(getpid(), cpu_burst, 0, msgq_id);
            }
        }
    }
}