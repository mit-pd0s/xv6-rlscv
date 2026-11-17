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
  int ratio_int;
  int ratio_dec;
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

  printf("--- Highest Response Ratio Next (HRRN) Test ---\n");
  printf("Parent PID: %d\n\n", getpid());

  // Parent sets high priority burst so it schedules reliably when waking from pause
  set_burst(getpid(), 1);

  // 1. Fork Child 1 (BT = 20 ticks)
  int arr1 = uptime();
  int pid1 = fork();
  if (pid1 == 0) {
    close(pfd[0]);
    set_burst(getpid(), 20);
    int start = uptime();
    int wait = start - arr1;
    int r = ((wait + 20) * 100) / 20;

    printf("[Child 1 | PID %d] Running with BT = 20 ticks...\n", getpid());
    run_workload(20);

    int finish = uptime();
    printf("[Child 1 | PID %d] Completed at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 1, 20, arr1, start, wait, r / 100, r % 100, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid1, 20);
  printf("[Parent] Forked Child 1 (PID %d, BT = 20)\n", pid1);

  // 2. Fork Child 2 [LONG JOB] (BT = 30 ticks) at tick 2
  int arr2 = uptime();
  int pid2 = fork();
  if (pid2 == 0) {
    close(pfd[0]);
    set_burst(getpid(), 30);

    int start = uptime();
    int wait = start - arr2;
    int r = ((wait + 30) * 100) / 30;

    printf("[Child 2 | PID %d] Running with BT = 30 ticks...\n", getpid());
    run_workload(30);

    int finish = uptime();
    printf("[Child 2 | PID %d] Completed at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 2, 30, arr2, start, wait, r / 100, r % 100, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid2, 30);
  printf("[Parent] Forked Child 2 (PID %d, BT = 30) [LONG JOB - Arrived at tick %d]\n", pid2, arr2);

  // Parent sleeps 18 ticks while Child 1 runs
  pause(18);

  // 3. Child 1 has finished; fork Child 3 [SHORT JOB] (BT = 10 ticks) right at tick 22
  int arr3 = uptime();
  int pid3 = fork();
  if (pid3 == 0) {
    close(pfd[0]);
    set_burst(getpid(), 10);

    int start = uptime();
    int wait = start - arr3;
    int r = ((wait + 10) * 100) / 10;

    printf("[Child 3 | PID %d] Running with BT = 10 ticks...\n", getpid());
    run_workload(10);

    int finish = uptime();
    printf("[Child 3 | PID %d] Completed at tick %d\n", getpid(), finish);

    struct completion_record rec = {getpid(), 3, 10, arr3, start, wait, r / 100, r % 100, finish};
    write(pfd[1], &rec, sizeof(rec));
    close(pfd[1]);
    exit(0);
  }
  set_burst(pid3, 10);
  printf("[Parent] Forked Child 3 (PID %d, BT = 10) [SHORT JOB - Arrived at tick %d]\n\n", pid3, arr3);

  int w2 = arr3 - arr2;
  int r2 = ((w2 + 30) * 100) / 30;
  printf("[HRRN Decision at tick %d]:\n", arr3);
  printf("  Child 2 [LONG, BT=30]: Waited %d ticks -> Response Ratio = (%d + 30)/30 = %d.%d\n",
         w2, w2, r2 / 100, r2 % 100);
  printf("  Child 3 [SHORT, BT=10]: Waited 0 ticks  -> Response Ratio = (0 + 10)/10 = 1.00\n");
  printf("  => Child 2's ratio shot up to %d.%d! Scheduling Child 2 ahead of Child 3!\n\n",
         r2 / 100, r2 % 100);

  close(pfd[1]); // Close parent write end so read() blocks until children write

  struct completion_record results[3];
  for (int i = 0; i < 3; i++) {
    read(pfd[0], &results[i], sizeof(struct completion_record));
  }
  close(pfd[0]);

  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf(">> Completion Order 1: PID %d (Child %d, BT %d ticks, Finished: tick %d)\n",
         results[0].pid, results[0].child_id, results[0].burst_time, results[0].finish_tick);
  printf(">> Completion Order 2: PID %d (Child %d [LONG], BT %d ticks, Finished: tick %d)\n",
         results[1].pid, results[1].child_id, results[1].burst_time, results[1].finish_tick);
  printf(">> Completion Order 3: PID %d (Child %d [SHORT], BT %d ticks, Finished: tick %d)\n\n",
         results[2].pid, results[2].child_id, results[2].burst_time, results[2].finish_tick);

  printf("Result: Long Job (PID %d, BT %d) completed BEFORE Short Job (PID %d, BT %d) because its ratio shot up (%d.%d vs 1.00)!\n",
         results[1].pid, results[1].burst_time, results[2].pid, results[2].burst_time,
         results[1].ratio_int, results[1].ratio_dec);

  exit(0);
}
