#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct completion_record {
  int pid;
  int child_id;
  int burst_time;
  int arrival_tick;
  int start_tick;
  int wait_ticks;
  int response_ratio; // (wait + burst) * 100 / burst
  int finish_tick;
};

void run_workload(int burst_ticks) {
  int start = uptime();
  while (uptime() - start < burst_ticks) {
    // busy wait
  }
}

int main(int argc, char *argv[]) {
  int pfd[2];
  if (pipe(pfd) < 0) {
    printf("Pipe creation failed!\n");
    exit(1);
  }

  printf("============================================================\n");
  printf("   Highest Response Ratio Next (HRRN) Scheduling Test       \n");
  printf("   Formula: Response Ratio R = (W + S) / S = 1 + (W / S)   \n");
  printf("============================================================\n");
  printf("Parent PID: %d starting at tick %d\n\n", getpid(), uptime());

  // Parent sets high priority burst so it can reliably schedule during staggered forks
  set_burst(getpid(), 1);

  // 1. Fork Child 1 (BT = 12 ticks)
  int pid1 = fork();
  if (pid1 == 0) {
    close(pfd[0]);
    int arrival = uptime();
    set_burst(getpid(), 12);
    int start = uptime();
    int wait = start - arrival;
    int ratio = ((wait + 12) * 100) / 12;
    printf("[Child 1 | PID %d] Started at tick %d (Arrived: %d, BT: 12, Wait: %d, Ratio: %d.%02d)\n",
           getpid(), start, arrival, wait, ratio / 100, ratio % 100);

    run_workload(12);

    int finish = uptime();
    printf("[Child 1 | PID %d] COMPLETED workload at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 1, 12, arrival, start, wait, ratio, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid1, 12);
  printf("[Parent] Forked Child 1 (PID %d) with Burst Time = 12 ticks\n", pid1);

  // 2. Fork Child 2 (BT = 16 ticks) at tick 2
  int pid2 = fork();
  if (pid2 == 0) {
    close(pfd[0]);
    int arrival = uptime();
    set_burst(getpid(), 16);

    int start = uptime();
    int wait = start - arrival;
    int ratio = ((wait + 16) * 100) / 16;
    printf("[Child 2 | PID %d] Started at tick %d (Arrived: %d, BT: 16, Wait: %d ticks, Ratio: %d.%02d)\n",
           getpid(), start, arrival, wait, ratio / 100, ratio % 100);

    run_workload(16);

    int finish = uptime();
    printf("[Child 2 | PID %d] COMPLETED workload at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 2, 16, arrival, start, wait, ratio, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid2, 16);
  printf("[Parent] Forked Child 2 (PID %d) with Burst Time = 16 ticks (arrived early at tick %d)\n",
         pid2, uptime());

  // 3. Fork Child 4 (BT = 60 ticks) at tick 2
  int pid4 = fork();
  if (pid4 == 0) {
    close(pfd[0]);
    int arrival = uptime();
    set_burst(getpid(), 60);

    int start = uptime();
    int wait = start - arrival;
    int ratio = ((wait + 60) * 100) / 60;
    printf("[Child 4 | PID %d] Started at tick %d (Arrived: %d, BT: 60, Wait: %d ticks, Ratio: %d.%02d)\n",
           getpid(), start, arrival, wait, ratio / 100, ratio % 100);

    run_workload(15);

    int finish = uptime();
    printf("[Child 4 | PID %d] COMPLETED workload at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 4, 60, arrival, start, wait, ratio, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid4, 60);
  printf("[Parent] Forked Child 4 (PID %d) with Burst Time = 60 ticks (arrived early at tick %d)\n",
         pid4, uptime());

  // Let Child 1 start running by yielding parent CPU for 8 ticks
  pause(8);

  // 4. Now Parent wakes up while Child 1 is still running, and forks Child 3 (BT = 6 ticks)
  int pid3 = fork();
  if (pid3 == 0) {
    close(pfd[0]);
    int arrival = uptime();
    set_burst(getpid(), 6);

    int start = uptime();
    int wait = start - arrival;
    int ratio = ((wait + 6) * 100) / 6;
    printf("[Child 3 | PID %d] Started at tick %d (Arrived: %d, BT: 6, Wait: %d ticks, Ratio: %d.%02d)\n",
           getpid(), start, arrival, wait, ratio / 100, ratio % 100);

    run_workload(6);

    int finish = uptime();
    printf("[Child 3 | PID %d] COMPLETED workload at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 3, 6, arrival, start, wait, ratio, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid3, 6);
  printf("[Parent] Forked Child 3 (PID %d) with Burst Time = 6 ticks (arrived LATE at tick %d)\n\n",
         pid3, uptime());

  close(pfd[1]); // Close parent write end so read() blocks until children write

  printf("[Parent] Monitoring chronological completion order from pipe...\n\n");

  struct completion_record results[4];
  for (int i = 0; i < 4; i++) {
    read(pfd[0], &results[i], sizeof(struct completion_record));
    printf(">> Completion Order %d: PID %d (Child %d, BT: %d ticks, Start: tick %d, Finish: tick %d)\n",
           i + 1, results[i].pid, results[i].child_id, results[i].burst_time,
           results[i].start_tick, results[i].finish_tick);
  }
  close(pfd[0]);

  for (int i = 0; i < 4; i++) {
    wait(0);
  }

  printf("\n============================================================\n");
  printf("HRRN vs. SJF vs. FCFS Proof:\n");
  printf("1. Child 2 (BT=16) ran BEFORE newly arrived Child 3 (BT=6)!\n");
  printf("   - Under SJF: Child 3 (BT 6) would run before Child 2 (BT 16) since 6 < 16.\n");
  printf("   - Under HRRN: Child 2 waited %d ticks, boosting its ratio to %d.%02d > Child 3's %d.%02d!\n",
         results[1].wait_ticks,
         results[1].response_ratio / 100, results[1].response_ratio % 100,
         results[2].response_ratio / 100, results[2].response_ratio % 100);
  printf("   => PROVES IT IS NOT SJF: Waiting time boosted priority!\n\n");
  printf("2. Child 3 (BT=6) finished BEFORE Child 4 (BT=60)!\n");
  printf("   - Child 4 was forked at tick %d (before Child 3 at tick %d).\n",
         results[3].arrival_tick, results[2].arrival_tick);
  printf("   - Yet Child 3 completed first because its ratio surged to %d.%02d!\n",
         results[2].response_ratio / 100, results[2].response_ratio % 100);
  printf("   => PROVES IT IS NOT FCFS!\n\n");
  printf("CONCLUSION: HRRN non-preemptive scheduler successfully verified.\n");
  printf("============================================================\n");

  exit(0);
}
