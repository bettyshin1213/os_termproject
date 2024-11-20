// message_queue.c
#include "utils.h"
#include "message_queue.h"

void send_message(int pid, int burst_value, int is_io, int msgq_id) {
    msgbuf_t msg;
    msg.mtype = pid;
    msg.burst_value = burst_value;
    msg.is_io = is_io;
    msgsnd(msgq_id, &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT);
}

int receive_message(msgbuf_t *msg, int msgq_id) {
    return msgrcv(msgq_id, msg, sizeof(*msg) - sizeof(long), 0, IPC_NOWAIT);
}