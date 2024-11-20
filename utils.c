#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"



void log_state(int output_fd, int tick, int pid, int remaining_burst, int *run_queue, int run_queue_count, int *wait_queue, int wait_queue_count) {
    char buffer[1024];  // Buffer to hold the formatted output
    int length = 0;

    // Format the main scheduling log line with tick, pid, and remaining burst time
    length += snprintf(buffer + length, sizeof(buffer) - length, "At time %d, process %d gets CPU time, remaining CPU-burst: %d\n", tick, pid, remaining_burst);

    // Add the run queue to the buffer
    length += snprintf(buffer + length, sizeof(buffer) - length, "Run Queue: ");
    for (int i = 0; i < run_queue_count; i++) {
        length += snprintf(buffer + length, sizeof(buffer) - length, "%d ", run_queue[i]);
    }
    length += snprintf(buffer + length, sizeof(buffer) - length, "\n");

    // Add the wait queue to the buffer
    length += snprintf(buffer + length, sizeof(buffer) - length, "Wait Queue: ");
    for (int i = 0; i < wait_queue_count; i++) {
        length += snprintf(buffer + length, sizeof(buffer) - length, "%d ", wait_queue[i]);
    }
    length += snprintf(buffer + length, sizeof(buffer) - length, "\n\n");


    // Ensure we don't write more than the buffer size
    if (length >= sizeof(buffer)) {
        fprintf(stderr, "Warning: log_state buffer overflow detected\n");
        return;
    }

    // Write the formatted string to the file using the file descriptor
    if (write(output_fd, buffer, length) != length) {
        perror("Error writing to schedule_dump.txt");
    }
}