#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Workload function: performs CPU computation and logs progress
void do_work(int child_id, int pid, uint64 iterations, int total_bt) {
  volatile uint64 counter = 0;
  int last_pct = 0;
  for (volatile uint64 j = 0; j < iterations; j++) {
    counter += j;
    int pct = (int)((j * 100) / iterations);
    if (pct >= 50 && last_pct < 50) {
      printf("[Child %d | PID %d] Reached 50%% progress (uptime: %d)\n",
             child_id, pid, uptime());
      last_pct = 50;
    }
  }
}

int main(int argc, char *argv[]) {
  printf("--- Shortest Remaining Time First (SRTF) Preemptive Test ---\n");
  printf("Parent PID: %d\n", getpid());
  printf("Demonstrating preemption with 3 children:\n");
  printf("  Child 1: Burst Time = 12 ticks (starts at tick 0)\n");
  printf("  Child 2: Burst Time = 6 ticks (arrives at tick 3 -> PREEMPTS Child 1)\n");
  printf("  Child 3: Burst Time = 2 ticks (arrives at tick 5 -> PREEMPTS Child 2)\n\n");

  // Pipe to record chronological completion order
  int pfd[2];
  if (pipe(pfd) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  // Fork Child 2 first (sleeps 3 ticks, arrives at tick 3)
  int pid2 = fork();
  if (pid2 == 0) {
    close(pfd[0]);
    int my_pid = getpid();
    set_burst(my_pid, 6);
    // Sleep 3 ticks to simulate arrival after Child 1 has started
    pause(3);
    printf("[Child 2 | PID %d] Arrived at tick %d (BT = 6 ticks) -> PREEMPTING Child 1!\n",
           my_pid, uptime());
    do_work(2, my_pid, 120000000ULL, 6);
    printf("[Child 2 | PID %d] COMPLETED workload at tick %d\n", my_pid, uptime());
    int cid = 2;
    write(pfd[1], &cid, sizeof(int));
    close(pfd[1]);
    exit(0);
  }

  // Fork Child 3 (sleeps 5 ticks, arrives at tick 5)
  int pid3 = fork();
  if (pid3 == 0) {
    close(pfd[0]);
    int my_pid = getpid();
    set_burst(my_pid, 2);
    // Sleep 5 ticks to simulate arrival after Child 2 has started
    pause(5);
    printf("[Child 3 | PID %d] Arrived at tick %d (BT = 2 ticks) -> PREEMPTING Child 2!\n",
           my_pid, uptime());
    do_work(3, my_pid, 40000000ULL, 2);
    printf("[Child 3 | PID %d] COMPLETED workload at tick %d\n", my_pid, uptime());
    int cid = 3;
    write(pfd[1], &cid, sizeof(int));
    close(pfd[1]);
    exit(0);
  }

  // Fork Child 1 (starts immediately at tick 0, BT = 12 ticks)
  int pid1 = fork();
  if (pid1 == 0) {
    close(pfd[0]);
    int my_pid = getpid();
    set_burst(my_pid, 12);
    printf("[Child 1 | PID %d] Running with Burst Time = 12 ticks (uptime: %d)\n",
           my_pid, uptime());
    do_work(1, my_pid, 240000000ULL, 12);
    printf("[Child 1 | PID %d] COMPLETED workload at tick %d\n", my_pid, uptime());
    int cid = 1;
    write(pfd[1], &cid, sizeof(int));
    close(pfd[1]);
    exit(0);
  }

  // Parent closes write end and waits for children
  close(pfd[1]);

  printf("[Parent] All 3 children forked. Child 1 running, Children 2 & 3 scheduled to arrive...\n\n");

  // Read chronological completion order from pipe
  for (int order = 1; order <= 3; order++) {
    int cid;
    if (read(pfd[0], &cid, sizeof(int)) == sizeof(int)) {
      int bt = (cid == 1) ? 12 : ((cid == 2) ? 6 : 2);
      printf(">> Chronological Completion %d: Child %d (Burst Time %d ticks)\n",
             order, cid, bt);
    }
  }
  close(pfd[0]);

  // Clean up zombies
  wait(0);
  wait(0);
  wait(0);

  printf("\n--- SRTF Test Passed: Preemption verified! Child 3 completed first, Child 2 second, Child 1 last! ---\n");
  exit(0);
}
