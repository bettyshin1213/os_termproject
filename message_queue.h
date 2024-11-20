// message_queue.h
#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "utils.h"

// Function to send a message to a child or parent process
void send_message(int pid, int burst_value, int is_io, int msgq_id);

// Function to receive a message from the message queue
int receive_message(msgbuf_t *msg, int msgq_id);

#endif