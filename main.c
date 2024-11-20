#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "utils.h"
#include "scheduler.h"
#include "message_queue.h"
#include "process.h"

int run_queue[NUM_CHILDREN];
int wait_queue[NUM_CHILDREN];
int run_queue_count = 0;
int wait_queue_count = 0;
int current_tick = 0;
int current_process = -1;
int msgq_id;
int output_fd;

void handle_alarm(int sig);


int main() {
    printf("Starting the scheduler program...\n");


    // 메시지 큐 생성
    msgq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msgq_id == -1) {
        perror("msgget failed");
        exit(1);
    }

    // schedule_dump.txt 파일을 열기
    output_fd = open("schedule_dump.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Error opening schedule_dump.txt");
        return 1;
    }

    // 타이머 설정 (100ms 마다 알람 발생)
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = MAX_TIME_TICKS; // 10ms = 100 ticks per second
    timer.it_interval = timer.it_value;
    signal(SIGALRM, handle_alarm);
    setitimer(ITIMER_REAL, &timer, NULL);
    printf("Timer set up successfully.\n");

    // 자식 프로세스 생성
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            srand(getpid());
            int cpu_burst = rand() % CPU_BURST_MAX + 1;
            int io_burst = rand() % IO_BURST_MAX + 1;
            child_process(cpu_burst, io_burst, msgq_id); // 자식 프로세스 실행
            exit(0);
        } else {
            add_to_queue(pid, run_queue, &run_queue_count);
        }
    }

    // 스케줄링 루프 (10,000 time ticks 동안 실행)
    while (current_tick < MAX_TIME_TICKS) {
        pause();
    }

    // 메시지 큐 제거
    msgctl(msgq_id, IPC_RMID, NULL);

    // 출력 파일 닫기
    close(output_fd);

    return 0;
}

void handle_alarm(int sig) {
    current_tick++;
    //printf("Handling alarm signal, tick: %d\n", current_tick);
    schedule(run_queue, &run_queue_count, wait_queue, &wait_queue_count, msgq_id, &current_process, output_fd, current_tick);
}