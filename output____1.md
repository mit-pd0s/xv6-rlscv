# xv6 Lab Assignments — Actual Live Terminal Outputs

This cheatsheet contains the **actual, live, unedited terminal outputs** captured directly from running each test in QEMU across all 29 assignment folders.

---

## Lab 3: CPU Scheduling Algorithms (Set 1)

### `fcfs` — First-Come First-Served Scheduler
**Command**: `make -C xv6-riscv/fcfs qemu` then `$ fcfstest`

```text
$ fcfstest
--- FCFS Scheduler Test Starting ---
Parent PID: 3
Forking 3 CPU-bound children with staggered arrival...
[Parent] Forked Child 1 (PID 4)
[Parent] Forked Child 2 (PID 5)
[Parent] Forked Child 3 (PID 6)

[Parent] All 3 children forked. Waiting for children to complete...

[Child 1 | PID 4] Created, starting CPU-bound workload...
[Child 1 | PID 4] Finished computation (counter=1283106752)
>> Completion Order 1: PID 4 (Child 1)
[Child 2 | PID 5] Created, starting CPU-bound workload...
[Child 2 | PID 5] Finished computation (counter=1283106752)
>> Completion Order 2: PID 5 (Child 2)
[Child 3 | PID 6] Created, starting CPU-bound workload...
[Child 3 | PID 6] Finished computation (counter=1283106752)
>> Completion Order 3: PID 6 (Child 3)

--- FCFS Test Passed: Processes completed strictly in creation order! ---
```

### `priority_aging` — Priority Preemptive Scheduler with Aging
**Command**: `make -C xv6-riscv/priority_aging qemu` then `$ priotest`

```text
$ priotest
============================================================
 Priority Preemptive Scheduler with Dynamic Aging Test     
 Priorities: 1 = Highest, 20 = Lowest. Default = 10         
 Aging Rule: After > 30 ticks waiting, Priority decreases by 1
============================================================
Parent PID: 3 at tick 2

Forking 4 processes in REVERSE priority order:
  Child 1: Forked 1st -> Priority 20 (Lowest)
  Child 2: Forked 2nd -> Priority 12 (Medium-Low)
  Child 3: Forked 3rd -> Priority 8  (Medium-High)
  Child 4: Forked 4th -> Priority 5  (Highest)

Under FCFS: Execution order would be Child 1 -> 2 -> 3 -> 4.
Under Priority Preemptive: Child 4 (Prio 5) MUST run FIRST!

[Parent] Forked Child 1 (PID 4) assigned Priority 20
[Parent] Forked Child 2 (PID 5) assigned Priority 12
[Parent] Forked Child 3 (PID 6) assigned Priority 8
[Parent] Forked Child 4 (PID 7) assigned Priority 5

[Parent] All 4 children forked. Monitoring execution & aging...

[Child 4 | PID 7] Started at tick 2 (Initial Prio: 5, Effective Prio: 5)
[Kernel Aging] PID 4 waited > 30 ticks: Priority boosted 20 -> 19
[Kernel Aging] PID 5 waited > 30 ticks: Priority boosted 12 -> 11
[Kernel Aging] PID 6 waited > 30 ticks: Priority boosted 8 -> 7
[Child 4 | PID 7] COMPLETED at tick 37 (Initial: 5 -> Final: 5)
[Child 3 | PID 6] Started at tick 37 (Initial Prio: 8, Effective Prio: 7)
[Kernel Aging] PID 4 waited > 30 ticks: Priority boosted 19 -> 18
[Kernel Aging] PID 5 waited > 30 ticks: Priority boosted 11 -> 10
[Kernel Aging] PID 3 waited > 30 ticks: Priority boosted 10 -> 9
[Child 3 | PID 6] COMPLETED at tick 72 (Initial: 8 -> Final: 7)

>> Completion Order 1: PID 7 (Child 4, Init Prio: 5, Final Prio: 5, Ran: tick 2 to 37) [Unchanged]

>> Completion Order 2: PID 6 (Child 3, Init Prio: 8, Final Prio: 7, Ran: tick 37 to 72) [AGED UP BY 1 LEVELS!]
[Child 2 | PID 5] Started at tick 72 (Initial Prio: 12, Effective Prio: 10)
[Child 2 | PID 5] COMPLETED at tick 92 (Initial: 12 -> Final: 10)

>> Completion Order 3: PID 5 (Child 2, Init Prio: 12, Final Prio: 10, Ran: tick 72 to 92) [AGED UP BY 2 LEVELS!]
[Child 1 | PID 4] Started at tick 92 (Initial Prio: 20, Effective Prio: 18)
[Child 1 | PID 4] COMPLETED at tick 107 (Initial: 20 -> Final: 18)

>> Completion Order 4: PID 4 (Child 1, Init Prio: 20, Final Prio: 18, Ran: tick 92 to 107) [AGED UP BY 2 LEVELS!]

============================================================
Test Summary & Proof:
1. Priority Preemption Verified (NOT FCFS):
   - Child 4 (PID 7, Prio 5) was forked LAST, but completed FIRST!
   - Child 1 (PID 4, Prio 20) was forked FIRST, but completed LAST!
   - The completion order is the exact REVERSE of fork order, proving priority scheduling.

2. Anti-Starvation Dynamic Aging Verified:
   - Real-time kernel aging logs confirmed wait-tick thresholds (> 30 ticks).
   - Child 3 (PID 6): Waited while higher priorities ran; aged from 8 -> 7!
   - Child 2 (PID 5): Waited while higher priorities ran; aged from 12 -> 10!
   - Child 1 (PID 4): Waited while higher priorities ran; aged from 20 -> 18!
   - Lowest priority process (Initial Prio 20) was never starved and finished cleanly.
============================================================
```

### `sjf` — Shortest Job First Non-Preemptive Scheduler
**Command**: `make -C xv6-riscv/sjf qemu` then `$ sjftest`

```text
$ sjftest
--- Non-preemptive Shortest Job First (SJF) Test ---
Parent PID: 3
Forking 4 processes with declared burst times (40, 10, 30, 20 ticks)...

[Parent] Forked Child 1 (PID 4) with declared burst time = 40
[Parent] Forked Child 2 (PID 5) with declared burst time = 10
[Parent] Forked Child 3 (PID 6) with declared burst time = 30
[Parent] Forked Child 4 (PID 7) with declared burst time = 20

[Parent] All children forked. Waiting for completion...

[Child 2 | PID 5] Declared burst time = 10 ticks (uptime: 2)
[Child 2 | PID 5 | BT 10] Completed workload at tick 4
[Child 4 | PID 7] Declared burst time = 20 ticks (uptime: 4)
[Child 4 | PID 7 | BT 20] Completed workload at tick 6
[Child 3 | PID 6] Declared burst time = 30 ticks (uptime: 6)
[Child 3 | PID 6 | BT 30] Completed workload at tick 9
[Child 1 | PID 4] Declared burst time = 40 ticks (uptime: 9)
[Child 1 | PID 4 | BT 40] Completed workload at tick 11
>> Completion Order 1: PID 4 (Child 1, Declared Burst Time 40 ticks)
>> Completion Order 2: PID 5 (Child 2, Declared Burst Time 10 ticks)
>> Completion Order 3: PID 6 (Child 3, Declared Burst Time 30 ticks)
>> Completion Order 4: PID 7 (Child 4, Declared Burst Time 20 ticks)

--- SJF Test Passed: Processes completed strictly in ascending order of declared burst time! ---
```

---

## Lab 4: CPU Scheduling Algorithms (Set 2)

### `srtf` — Shortest Remaining Time First Preemptive Scheduler
**Command**: `make -C xv6-riscv/srtf qemu` then `$ srtftest`

```text
$ srtftest
--- Shortest Remaining Time First (SRTF) Preemptive Test ---
Parent PID: 3
Demonstrating preemption with 3 children:
  Child 1: Burst Time = 12 ticks (starts at tick 0)
  Child 2: Burst Time = 6 ticks (arrives at tick 3 -> PREEMPTS Child 1)
  Child 3: Burst Time = 2 ticks (arrives at tick 5 -> PREEMPTS Child 2)

[Parent] All 3 children forked. Child 1 running, Children 2 & 3 scheduled to arrive...

[Child 1 | PID 6] Running with Burst Time = 12 ticks (uptime: 2)
[Child 2 | PID 4] Arrived at tick 5 (BT = 6 ticks) -> PREEMPTING Child 1!
[Child 3 | PID 5] Arrived at tick 7 (BT = 2 ticks) -> PREEMPTING Child 2!
[Child 3 | PID 5] Reached 50% progress (uptime: 8)
[Child 3 | PID 5] COMPLETED workload at tick 9
[Child 2 | PID 4] Reached 50% progress (uptime: 11)
[Child 2 | PID 4] COMPLETED workload at tick 14
[Child 1 | PID 6] Reached 50% progress (uptime: 19)
[Child 1 | PID 6] COMPLETED workload at tick 27
>> Chronological Completion 1: Child 3 (Burst Time 2 ticks)
>> Chronological Completion 2: Child 2 (Burst Time 6 ticks)
>> Chronological Completion 3: Child 1 (Burst Time 12 ticks)

--- SRTF Test Passed: Preemption verified! Child 3 completed first, Child 2 second, Child 1 last! ---
```

### `hrrn` — Highest Response Ratio Next Non-Preemptive Scheduler
**Command**: `make -C xv6-riscv/hrrn qemu` then `$ hrrntest`

```text
$ hrrntest
--- Highest Response Ratio Next (HRRN) Test ---
Parent PID: 3

[Parent] Forked Child 1 (PID 4, BT = 20)
[Parent] Forked Child 2 (PID 5, BT = 30) [LONG JOB - Arrived at tick 2]
[Child 1 | PID 4] Running with BT = 20 ticks...
[Child 1 | PID 4] Completed at tick 22
[Parent] Forked Child 3 (PID 6, BT = 10) [SHORT JOB - Arrived at tick 22]

[HRRN Decision at tick 22]:
  Child 2 [LONG, BT=30]: Waited 20 ticks -> Response Ratio = (20 + 30)/30 = 1.66
  Child 3 [SHORT, BT=10]: Waited 0 ticks  -> Response Ratio = (0 + 10)/10 = 1.00
  => Child 2's ratio shot up to 1.66! Scheduling Child 2 ahead of Child 3!

[Child 2 | PID 5] Running with BT = 30 ticks...
[Child 2 | PID 5] Completed at tick 52
[Child 3 | PID 6] Running with BT = 10 ticks...
[Child 3 | PID 6] Completed at tick 62
>> Completion Order 1: PID 4 (Child 1, BT 20 ticks, Finished: tick 22)
>> Completion Order 2: PID 5 (Child 2 [LONG], BT 30 ticks, Finished: tick 52)
>> Completion Order 3: PID 6 (Child 3 [SHORT], BT 10 ticks, Finished: tick 62)

Result: Long Job (PID 5, BT 30) completed BEFORE Short Job (PID 6, BT 10) because its ratio shot up (1.66 vs 1.00)!
```

### `mlfq` — Multilevel Feedback Queue Scheduler
**Command**: `make -C xv6-riscv/mlfq qemu` then `$ mlfqtest`

```text
$ mlfqtest
--- Multilevel Feedback Queue (MLFQ) Scheduler Test ---
Parent PID: 3
Forking CPU-bound process (demotes to level 2) and I/O-bound process (remains at level 0)...

[CPU-bound | PID 4] Initial Queue Level: 0 (uptime: 2)
[I/O-bound | PID 5] Initial Queue Level: 0 (uptime: 3)
[CPU-bound | PID 4] After Step 1 computation: Queue Level = 1 (uptime: 3)
[I/O-bound | PID 5] After Step 1 I/O burst: Queue Level = 0 (remains at high priority!)
[CPU-bound | PID 4] After Step 2 computation: Queue Level = 2 (uptime: 5)
[CPU-bound | PID 4] After Step 3 computation: Queue Level = 2 (uptime: 7)
[CPU-bound | PID 4] Final Queue Level = 2 (successfully sank to lowest queue!)
[I/O-bound | PID 5] After Step 2 I/O burst: Queue Level = 0 (remains at high priority!)
[I/O-bound | PID 5] After Step 3 I/O burst: Queue Level = 0 (remains at high priority!)
[I/O-bound | PID 5] After Step 4 I/O burst: Queue Level = 0 (remains at high priority!)
[I/O-bound | PID 5] After Step 5 I/O burst: Queue Level = 0 (remains at high priority!)
[I/O-bound | PID 5] Final Queue Level = 0 (retained top priority!)

--- MLFQ Test Passed: CPU-bound sank to lowest queue; I/O process remained in top queue! ---
```

---

## Lab 5: Process Concurrency & Synchronization

### `peterson` — Peterson's Algorithm Mutual Exclusion
**Command**: `make -C xv6-riscv/peterson qemu` then `$ peterson`

```text
$ peterson
--- Peterson's Algorithm Mutual Exclusion Test ---
Process 0 in CS, counter = 1
Process 0 in CS, counter = 2
Process 0 in CS, counter = 3
Process 0 in CS, counter = 4
Process 0 in CS, counter = 5
Process 0 in CS, counter = 6
Process 0 in CS, counter = 7
Process 1 in CS, counter = 8
Process 0 in CS, counter = 9
Process 1 in CS, counter = 10
Process 0 in CS, counter = 11
Process 1 in CS, counter = 12
Process 0 in CS, counter = 13
Process 1 in CS, counter = 14
Process 1 in CS, counter = 15
Process 1 in CS, counter = 16
Process 1 in CS, counter = 17
Process 1 in CS, counter = 18
Process 1 in CS, counter = 19
Process 1 in CS, counter = 20

Expected final counter = 20
Actual final counter   = 20
--- Peterson's Algorithm Test Passed: Mutual exclusion preserved, zero lost updates! ---
```

### `prodcons` — Producer-Consumer Bounded Buffer with Semaphores
**Command**: `make -C xv6-riscv/prodcons qemu` then `$ prodcons`

```text
$ prodcons
============================================================
Producer-Consumer Bounded Buffer Simulation with Semaphores
Buffer capacity = 5, Total items = 20
============================================================
PrConsumer started (pid 5)
Consumer: Buffer is EMPTY (0/5 slots), waiting on full semaphore...
oducer started (pid 4)
Producer [pid 4]: Produced item 1 | Buffer count: 1/5
Consumer [pid 5]: Consumed item 1 | Buffer count: 0/5
Producer [pid 4]: Produced item 2 | Buffer count: 1/5
Producer [pid 4]: Produced item 3 | Buffer count: 2/5
Consumer [pid 5]: Consumed item 2 | Buffer count: 1/5
Producer [pid 4]: Produced item 4 | Buffer count: 2/5
Producer [pid 4]: Produced item 5 | Buffer count: 3/5
Consumer [pid 5]: Consumed item 3 | Buffer count: 2/5
Producer [pid 4]: Produced item 6 | Buffer count: 3/5
Producer [pid 4]: Produced item 7 | Buffer count: 4/5
Consumer [pid 5]: Consumed item 4 | Buffer count: 3/5
Producer [pid 4]: Produced item 8 | Buffer count: 4/5
Producer [pid 4]: Produced item 9 | Buffer count: 5/5
Consumer [pid 5]: Consumed item 5 | Buffer count: 4/5
Producer [pid 4]: Produced item 10 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 6 | Buffer count: 4/5
Producer [pid 4]: Produced item 11 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 7 | Buffer count: 4/5
Producer [pid 4]: Produced item 12 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 8 | Buffer count: 4/5
Producer [pid 4]: Produced item 13 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 9 | Buffer count: 4/5
Producer [pid 4]: Produced item 14 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 10 | Buffer count: 4/5
Producer [pid 4]: Produced item 15 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 11 | Buffer count: 4/5
Producer [pid 4]: Produced item 16 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 12 | Buffer count: 4/5
Producer [pid 4]: Produced item 17 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 13 | Buffer count: 4/5
Producer [pid 4]: Produced item 18 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 14 | Buffer count: 4/5
Producer [pid 4]: Produced item 19 | Buffer count: 5/5
Producer: Buffer is FULL (5/5 slots), waiting on empty semaphore...
Consumer [pid 5]: Consumed item 15 | Buffer count: 4/5
Producer [pid 4]: Produced item 20 | Buffer count: 5/5
Producer finished producing 20 items.
Consumer [pid 5]: Consumed item 16 | Buffer count: 4/5
Consumer [pid 5]: Consumed item 17 | Buffer count: 3/5
Consumer [pid 5]: Consumed item 18 | Buffer count: 2/5
Consumer [pid 5]: Consumed item 19 | Buffer count: 1/5
Consumer [pid 5]: Consumed item 20 | Buffer count: 0/5
Consumer verification: ALL 20 items received correctly in FIFO order!
============================================================
Simulation Finished Summary:
Peak buffer usage: 5 / 5 slots
Min buffer usage: 0 / 5 slots
SUCCESS: No buffer overflow or underflow detected.
Buffer is cleanly drained (final count = 0).
============================================================
```

### `readwrite` — Readers-Writers Problem (Turnstile Method)
**Command**: `make -C xv6-riscv/readwrite qemu` then `$ readwrite`

```text
$ readwrite
============================================================
Readers-Writers Problem Simulation (Fair Turnstile - Starvation-Free)
Readers = 3, Writers = 2
Reads per reader = 5, Writes per writer = 5
============================================================
Reader 1 started (pid 4)
Reader 1 [pid 4]: read shared_data = 0 | active readers = 1
Reader 2 started (pid 5)
Reader 2 [pid 5]: read shared_data = 0 | active readers = 2
Reader 3 started (pid 6)
Reader 3 [pid 6]: read shared_data = 0 | active readers = 3
Writer 1 started (pid 7)
Writer 2 started (pid 8)
Writer 1 [pid 7]: wrote shared_data = 1 | active readers = 0, active writers = 1
Writer 2 [pid 8]: wrote shared_data = 2 | active readers = 0, active writers = 1
Reader 1 [pid 4]: read shared_data = 2 | active readers = 1
Reader 2 [pid 5]: read shared_data = 2 | active readers = 2
Reader 3 [pid 6]: read shared_data = 2 | active readers = 3
Writer 1 [pid 7]: wrote shared_data = 3 | active readers = 0, active writers = 1
Writer 2 [pid 8]: wrote shared_data = 4 | active readers = 0, active writers = 1
Reader 1 [pid 4]: read shared_data = 4 | active readers = 1
Reader 2 [pid 5]: read shared_data = 4 | active readers = 2
Reader 3 [pid 6]: read shared_data = 4 | active readers = 3
Writer 1 [pid 7]: wrote shared_data = 5 | active readers = 0, active writers = 1
Writer 2 [pid 8]: wrote shared_data = 6 | active readers = 0, active writers = 1
Reader 1 [pid 4]: read shared_data = 6 | active readers = 1
Reader 2 [pid 5]: read shared_data = 6 | active readers = 2
Reader 3 [pid 6]: read shared_data = 6 | active readers = 3
Writer 1 [pid 7]: wrote shared_data = 7 | active readers = 0, active writers = 1
Writer 2 [pid 8]: wrote shared_data = 8 | active readers = 0, active writers = 1
Reader 1 [pid 4]: read shared_data = 8 | active readers = 1
Reader 2 [pid 5]: read shared_data = 8 | active readers = 2
Reader 3 [pid 6]: read shared_data = 8 | active readers = 3
Writer 1 [pid 7]: wrote shared_data = 9 | active readers = 0, active writers = 1
Reader 1 [pid 4] finished all 5 reads.
Reader 2 [pid 5] finished all 5 reads.
Reader 3 [pid 6] finished all 5 reads.
Writer 2 [pid 8]: wrote shared_data = 10 | active readers = 0, active writers = 1
Writer 1 [pid 7] finished all 5 writes.
Writer 2 [pid 8] finished all 5 writes.
============================================================
Simulation Finished Summary:
Final shared_data value: 10 (Expected: 10)
Total writes recorded: 10
Max concurrent readers observed: 3
Synchronization violations: 0
SUCCESS:
  1. Multiple readers read simultaneously (max = 3).
  2. Writers had exclusive access (read_count = 0 and active_writers = 1).
  3. No lost updates or race conditions occurred.
============================================================
```

### `dining` — Dining Philosophers with Deadlock Avoidance
**Command**: `make -C xv6-riscv/dining qemu` then `$ dining`

```text
$ dining
============================================================
Dining Philosophers Simulation with Deadlock Avoidance
Philosophers = 5, Forks = 5, Cycles = 5
Strategy: Resource Ordering (acquire lower-numbered fork first)
============================================================
Philosopher 0 [pid 4]: THINKING -> HUNGRY (Cycle 1/5)
Philosopher 0 [pid 4]: picked up fork 0, waiting for fork 1
Philosopher 0 [pid 4]: HUNGRY -> EATING (holding forks 0, 1) [Meal #1]
Philosopher 1 [pid 5]: THINKING -> HUNGRY (Cycle 1/5)
Philosopher 2 [pid 6]: THINKING -> HUNGRY (Cycle 1/5)
Philosopher 2 [pid 6]: picked up fork 2, waiting for fork 3
Philosopher 2 [pid 6]: HUNGRY -> EATING (holding forks 2, 3) [Meal #1]
Philosopher 3 [pid 7]: THINKING -> HUNGRY (Cycle 1/5)
Philosopher 4 [pid 8]: THINKING -> HUNGRY (Cycle 1/5)
Philosopher 0 [pid 4]: EATING -> THINKING (released forks 0, 1)
Philosopher 1 [pid 5]: picked up fork 1, waiting for fork 2
Philosopher 2 [pid 6]: EATING -> THINKING (released forks 2, 3)
Philosopher 3 [pid 7]: picked up fork 3, waiting for fork 4
Philosopher 3 [pid 7]: HUNGRY -> EATING (holding forks 3, 4) [Meal #1]
Philosopher 4 [pid 8]: picked up fork 0, waiting for fork 4
Philosopher 1 [pid 5]: HUNGRY -> EATING (holding forks 1, 2) [Meal #1]
Philosopher 0 [pid 4]: THINKING -> HUNGRY (Cycle 2/5)
Philosopher 2 [pid 6]: THINKING -> HUNGRY (Cycle 2/5)
Philosopher 1 [pid 5]: EATING -> THINKING (released forks 1, 2)
Philosopher 2 [pid 6]: picked up fork 2, waiting for fork 3
Philosopher 3 [pid 7]: EATING -> THINKING (released forks 3, 4)
Philosopher 4 [pid 8]: HUNGRY -> EATING (holding forks 0, 4) [Meal #1]
Philosopher 2 [pid 6]: HUNGRY -> EATING (holding forks 2, 3) [Meal #2]
Philosopher 1 [pid 5]: THINKING -> HUNGRY (Cycle 2/5)
Philosopher 1 [pid 5]: picked up fork 1, waiting for fork 2
Philosopher 3 [pid 7]: THINKING -> HUNGRY (Cycle 2/5)
Philosopher 2 [pid 6]: EATING -> THINKING (released forks 2, 3)
Philosopher 3 [pid 7]: picked up fork 3, waiting for fork 4
Philosopher 4 [pid 8]: EATING -> THINKING (released forks 0, 4)
Philosopher 0 [pid 4]: picked up fork 0, waiting for fork 1
Philosopher 1 [pid 5]: HUNGRY -> EATING (holding forks 1, 2) [Meal #2]
Philosopher 3 [pid 7]: HUNGRY -> EATING (holding forks 3, 4) [Meal #2]
Philosopher 2 [pid 6]: THINKING -> HUNGRY (Cycle 3/5)
Philosopher 4 [pid 8]: THINKING -> HUNGRY (Cycle 2/5)
Philosopher 1 [pid 5]: EATING -> THINKING (released forks 1, 2)
Philosopher 2 [pid 6]: picked up fork 2, waiting for fork 3
Philosopher 3 [pid 7]: EATING -> THINKING (released forks 3, 4)
Philosopher 0 [pid 4]: HUNGRY -> EATING (holding forks 0, 1) [Meal #2]
Philosopher 2 [pid 6]: HUNGRY -> EATING (holding forks 2, 3) [Meal #3]
Philosopher 1 [pid 5]: THINKING -> HUNGRY (Cycle 3/5)
Philosopher 3 [pid 7]: THINKING -> HUNGRY (Cycle 3/5)
Philosopher 0 [pid 4]: EATING -> THINKING (released forks 0, 1)
Philosopher 1 [pid 5]: picked up fork 1, waiting for fork 2
Philosopher 2 [pid 6]: EATING -> THINKING (released forks 2, 3)
Philosopher 3 [pid 7]: picked up fork 3, waiting for fork 4
Philosopher 3 [pid 7]: HUNGRY -> EATING (holding forks 3, 4) [Meal #3]
Philosopher 4 [pid 8]: picked up fork 0, waiting for fork 4
Philosopher 1 [pid 5]: HUNGRY -> EATING (holding forks 1, 2) [Meal #3]
Philosopher 0 [pid 4]: THINKING -> HUNGRY (Cycle 3/5)
Philosopher 2 [pid 6]: THINKING -> HUNGRY (Cycle 4/5)
Philosopher 1 [pid 5]: EATING -> THINKING (released forks 1, 2)
Philosopher 2 [pid 6]: picked up fork 2, waiting for fork 3
Philosopher 3 [pid 7]: EATING -> THINKING (released forks 3, 4)
Philosopher 4 [pid 8]: HUNGRY -> EATING (holding forks 0, 4) [Meal #2]
Philosopher 2 [pid 6]: HUNGRY -> EATING (holding forks 2, 3) [Meal #4]
Philosopher 1 [pid 5]: THINKING -> HUNGRY (Cycle 4/5)
Philosopher 1 [pid 5]: picked up fork 1, waiting for fork 2
Philosopher 3 [pid 7]: THINKING -> HUNGRY (Cycle 4/5)
Philosopher 2 [pid 6]: EATING -> THINKING (released forks 2, 3)
Philosopher 3 [pid 7]: picked up fork 3, waiting for fork 4
Philosopher 4 [pid 8]: EATING -> THINKING (released forks 0, 4)
Philosopher 0 [pid 4]: picked up fork 0, waiting for fork 1
Philosopher 1 [pid 5]: HUNGRY -> EATING (holding forks 1, 2) [Meal #4]
Philosopher 3 [pid 7]: HUNGRY -> EATING (holding forks 3, 4) [Meal #4]
Philosopher 2 [pid 6]: THINKING -> HUNGRY (Cycle 5/5)
Philosopher 4 [pid 8]: THINKING -> HUNGRY (Cycle 3/5)
Philosopher 1 [pid 5]: EATING -> THINKING (released forks 1, 2)
Philosopher 2 [pid 6]: picked up fork 2, waiting for fork 3
Philosopher 3 [pid 7]: EATING -> THINKING (released forks 3, 4)
Philosopher 0 [pid 4]: HUNGRY -> EATING (holding forks 0, 1) [Meal #3]
Philosopher 2 [pid 6]: HUNGRY -> EATING (holding forks 2, 3) [Meal #5]
Philosopher 1 [pid 5]: THINKING -> HUNGRY (Cycle 5/5)
Philosopher 3 [pid 7]: THINKING -> HUNGRY (Cycle 5/5)
Philosopher 0 [pid 4]: EATING -> THINKING (released forks 0, 1)
Philosopher 1 [pid 5]: picked up fork 1, waiting for fork 2
Philosopher 2 [pid 6]: EATING -> THINKING (released forks 2, 3)
Philosopher 2 [pid 6]: Finished all 5 dining cycles.
Philosopher 3 [pid 7]: picked up fork 3, waiting for fork 4
Philosopher 3 [pid 7]: HUNGRY -> EATING (holding forks 3, 4) [Meal #5]
Philosopher 4 [pid 8]: picked up fork 0, waiting for fork 4
Philosopher 1 [pid 5]: HUNGRY -> EATING (holding forks 1, 2) [Meal #5]
Philosopher 0 [pid 4]: THINKING -> HUNGRY (Cycle 4/5)
Philosopher 1 [pid 5]: EATING -> THINKING (released forks 1, 2)
Philosopher 1 [pid 5]: Finished all 5 dining cycles.
Philosopher 3 [pid 7]: EATING -> THINKING (released forks 3, 4)
Philosopher 3 [pid 7]: Finished all 5 dining cycles.
Philosopher 4 [pid 8]: HUNGRY -> EATING (holding forks 0, 4) [Meal #3]
Philosopher 4 [pid 8]: EATING -> THINKING (released forks 0, 4)
Philosopher 0 [pid 4]: picked up fork 0, waiting for fork 1
Philosopher 0 [pid 4]: HUNGRY -> EATING (holding forks 0, 1) [Meal #4]
Philosopher 4 [pid 8]: THINKING -> HUNGRY (Cycle 4/5)
Philosopher 0 [pid 4]: EATING -> THINKING (released forks 0, 1)
Philosopher 4 [pid 8]: picked up fork 0, waiting for fork 4
Philosopher 4 [pid 8]: HUNGRY -> EATING (holding forks 0, 4) [Meal #4]
Philosopher 0 [pid 4]: THINKING -> HUNGRY (Cycle 5/5)
Philosopher 4 [pid 8]: EATING -> THINKING (released forks 0, 4)
Philosopher 0 [pid 4]: picked up fork 0, waiting for fork 1
Philosopher 0 [pid 4]: HUNGRY -> EATING (holding forks 0, 1) [Meal #5]
Philosopher 4 [pid 8]: THINKING -> HUNGRY (Cycle 5/5)
Philosopher 0 [pid 4]: EATING -> THINKING (released forks 0, 1)
Philosopher 0 [pid 4]: Finished all 5 dining cycles.
Philosopher 4 [pid 8]: picked up fork 0, waiting for fork 4
Philosopher 4 [pid 8]: HUNGRY -> EATING (holding forks 0, 4) [Meal #5]
Philosopher 4 [pid 8]: EATING -> THINKING (released forks 0, 4)
Philosopher 4 [pid 8]: Finished all 5 dining cycles.
============================================================
Simulation Finished Summary:
Philosopher 0 ate 5 / 5 meals.
Philosopher 1 ate 5 / 5 meals.
Philosopher 2 ate 5 / 5 meals.
Philosopher 3 ate 5 / 5 meals.
Philosopher 4 ate 5 / 5 meals.
Total meals eaten: 25 (Expected: 25)
Mutual exclusion violations: 0
SUCCESS:
  1. All 5 philosophers completed all 5 cycles without deadlocking.
  2. No two adjacent philosophers ate simultaneously.
  3. Resource ordering effectively eliminated circular wait.
============================================================
```

---

## Lab 6: Deadlock Avoidance, Detection & Prevention

### `bankers` — Banker's Algorithm Safety & Request
**Command**: `make -C xv6-riscv/bankers qemu` then `$ bankers`

```text
$ bankers
============================================================
      Banker's Algorithm Simulation for Deadlock Avoidance  
============================================================

--- Current System State ---
Process | Allocation |   Max      |   Need     
        | A   B   C  | A   B   C  | A   B   C  
-----------------------------------------------
  P0    | 0   1   0  | 7   5   3  | 7   4   3
  P1    | 2   0   0  | 3   2   2  | 1   2   2
  P2    | 3   0   2  | 9   0   2  | 6   0   0
  P3    | 2   1   1  | 2   2   2  | 0   1   1
  P4    | 0   0   2  | 4   3   3  | 4   3   1
Available: A = 3, B = 3, C = 2

[Test Case 1] Checking initial system safety...
System is in a SAFE state.
Safe execution sequence: <P1, P3, P4, P0, P2>

[Test Case 2] Process P1 requesting [1, 0, 2]...
>>> Process P1 requests resources: [A=1, B=0, C=2]
SUCCESS: Request granted safely!
Safe sequence: <P1, P3, P4, P0, P2>

--- Current System State ---
Process | Allocation |   Max      |   Need     
        | A   B   C  | A   B   C  | A   B   C  
-----------------------------------------------
  P0    | 0   1   0  | 7   5   3  | 7   4   3
  P1    | 3   0   2  | 3   2   2  | 0   2   0
  P2    | 3   0   2  | 9   0   2  | 6   0   0
  P3    | 2   1   1  | 2   2   2  | 0   1   1
  P4    | 0   0   2  | 4   3   3  | 4   3   1
Available: A = 2, B = 3, C = 0


[Test Case 3] Process P4 requesting [3, 3, 0]...
>>> Process P4 requests resources: [A=3, B=3, C=0]
Notice: Process P4 must wait. Not enough resources available (Request > Available).

[Test Case 4] Process P0 requesting [0, 2, 0]...
>>> Process P0 requests resources: [A=0, B=2, C=0]
DENIED: Request denied — would lead to unsafe state! Rolled back.

--- Current System State ---
Process | Allocation |   Max      |   Need     
        | A   B   C  | A   B   C  | A   B   C  
-----------------------------------------------
  P0    | 0   1   0  | 7   5   3  | 7   4   3
  P1    | 3   0   2  | 3   2   2  | 0   2   0
  P2    | 3   0   2  | 9   0   2  | 6   0   0
  P3    | 2   1   1  | 2   2   2  | 0   1   1
  P4    | 0   0   2  | 4   3   3  | 4   3   1
Available: A = 2, B = 3, C = 0

============================================================
Banker's Algorithm simulation completed successfully.
============================================================
```

### `deadlockdetect` — Deadlock Detection via Resource Allocation Graph
**Command**: `make -C xv6-riscv/deadlockdetect qemu` then `$ deadlockdetect`

```text
$ deadlockdetect
============================================================
   Deadlock Detection using Resource Allocation Graph (RAG) 
============================================================

============================================================
[Scenario 1] Acyclic Wait-For Graph (No Deadlock)
4 Processes (P0..P3), 4 Resources (R0..R3)
P0 holds R0, requests R1
P1 holds R1, requests R2
P2 holds R2 (no requests)
P3 holds R3, requests R2
============================================================
Resource Allocation Matrix:
Proc | R0 R1 R2 R3 
-----+------------
 P0  |  1  0  0  0 
 P1  |  0  1  0  0 
 P2  |  0  0  1  0 
 P3  |  0  0  0  1 

Resource Request Matrix:
Proc | R0 R1 R2 R3 
-----+------------
 P0  |  0  1  0  0 
 P1  |  0  0  1  0 
 P2  |  0  0  0  0 
 P3  |  0  0  1  0 

Constructed Wait-For Graph (WFG):
  Edge: P0 -> P1 (P0 waits for resource held by P1)
  Edge: P1 -> P2 (P1 waits for resource held by P2)
  Edge: P3 -> P2 (P3 waits for resource held by P2)
RESULT: NO DEADLOCK DETECTED.
The Wait-For Graph is acyclic. All processes can complete.

============================================================
[Scenario 2] Circular Wait among 4 processes (Deadlock Detected)
4 Processes (P0..P3), 4 Resources (R0..R3)
P0 holds R0, requests R1
P1 holds R1, requests R2
P2 holds R2, requests R3
P3 holds R3, requests R0
============================================================
Resource Allocation Matrix:
Proc | R0 R1 R2 R3 
-----+------------
 P0  |  1  0  0  0 
 P1  |  0  1  0  0 
 P2  |  0  0  1  0 
 P3  |  0  0  0  1 

Resource Request Matrix:
Proc | R0 R1 R2 R3 
-----+------------
 P0  |  0  1  0  0 
 P1  |  0  0  1  0 
 P2  |  0  0  0  1 
 P3  |  1  0  0  0 

Constructed Wait-For Graph (WFG):
  Edge: P0 -> P1 (P0 waits for resource held by P1)
  Edge: P1 -> P2 (P1 waits for resource held by P2)
  Edge: P2 -> P3 (P2 waits for resource held by P3)
  Edge: P3 -> P0 (P3 waits for resource held by P0)
RESULT: DEADLOCK DETECTED!
Cycle of deadlocked processes: P0 -> P1 -> P2 -> P3 -> P0

============================================================
Deadlock detection analysis finished.
============================================================
```

### `resourceorder` — Deadlock Prevention via Resource Ordering
**Command**: `make -C xv6-riscv/resourceorder qemu` then `$ resourceorder fixed`

```text
$ resourceorder fixed
============================================================
  SCENARIO 2: RESOURCE ORDERING (DEADLOCK PREVENTED)
  Global Order: Lock1 (Resource 0) < Lock2 (Resource 1)
  Both Process A & Process B acquire Lock1 THEN Lock2.
============================================================
[Tick 2] [Process A (pid 4)]: Attempting to acquire Lock 1...
[Tick 2] [Process A (pid 4)]: ACQUIRED Lock 1.
[Tick 3] [Process A (pid 4)]: Attempting to acquire Lock 2...
[Tick 3] [Process A (pid 4)]: ACQUIRED Lock 2. Entering Critical Section.
[Tick 3] [Process B (pid 5)]: Attempting to acquire Lock 1...
[Tick 5] [Process A (pid 4)]: Exiting Critical Section. Releasing Lock 2 and Lock 1.
[Tick 5] [Process A (pid 4)]: Completed successfully.
[Tick 5] [Process B (pid 5)]: ACQUIRED Lock 1.
[Tick 5] [Process B (pid 5)]: Attempting to acquire Lock 2...
[Tick 5] [Process B (pid 5)]: ACQUIRED Lock 2. Entering Critical Section.
[Tick 7] [Process B (pid 5)]: Exiting Critical Section. Releasing Lock 2 and Lock 1.
[Tick 7] [Process B (pid 5)]: Completed successfully.
============================================================
SUCCESS: Both Process A and Process B completed cleanly!
Deadlock was completely eliminated by enforcing global resource ordering.
============================================================
```

### `syncdeadlock` — Multi-Resource Sync & Deadlock Prevention
**Command**: `make -C xv6-riscv/syncdeadlock qemu` then `$ syncdeadlock`

```text
$ syncdeadlock
============================================================
 Multi-Resource Synchronization with Deadlock Avoidance   
 Processes: 5, Resources: Printer(2), Scanner(1), Disk(2)   
 Strategy : Resource Ordering (Hierarchy of Resources)      
 Cycles   : 3 per process (Total tasks = 15)               
============================================================
[Proc 0 (pid 4)]: Requesting Printer (first resource)...
[Proc 0 (pid 4)]: GRANTED Printer (In use: 1/2)
[Proc 0 (pid 4)]: Requesting Scanner (second resource)...
[Proc 0 (pid 4)]: GRANTED Scanner (In use: 1/1)
[Proc 0 (pid 4)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 1/3)...
[Proc 1 (pid 5)]: Requesting Scanner (first resource)...
[Proc 2 (pid 6)]: Requesting Printer (first resource)...
[Proc 2 (pid 6)]: GRANTED Printer (In use: 2/2)
[Proc 2 (pid 6)]: Requesting Disk (second resource)...
[Proc 2 (pid 6)]: GRANTED Disk (In use: 1/2)
[Proc 2 (pid 6)]: Holding [Printer, Disk]. STARTING WORK (Cycle 1/3)...
[Proc 3 (pid 7)]: Requesting Printer (first resource)...
[Proc 4 (pid 8)]: Requesting Scanner (first resource)...
[Proc 0 (pid 4)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 1/3 done).
[Proc 1 (pid 5)]: GRANTED Scanner (In use: 1/1)
[Proc 1 (pid 5)]: Requesting Disk (second resource)...
[Proc 1 (pid 5)]: GRANTED Disk (In use: 2/2)
[Proc 1 (pid 5)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 1/3)...
[Proc 2 (pid 6)]: FINISHED WORK. Released [Printer, Disk] (Cycle 1/3 done).
[Proc 3 (pid 7)]: GRANTED Printer (In use: 1/2)
[Proc 3 (pid 7)]: Requesting Scanner (second resource)...
[Proc 0 (pid 4)]: Requesting Printer (first resource)...
[Proc 0 (pid 4)]: GRANTED Printer (In use: 2/2)
[Proc 0 (pid 4)]: Requesting Scanner (second resource)...
[Proc 2 (pid 6)]: Requesting Printer (first resource)...
[Proc 1 (pid 5)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 1/3 done).
[Proc 3 (pid 7)]: GRANTED Scanner (In use: 1/1)
[Proc 3 (pid 7)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 1/3)...
[Proc 1 (pid 5)]: Requesting Scanner (first resource)...
[Proc 3 (pid 7)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 1/3 done).
[Proc 4 (pid 8)]: GRANTED Scanner (In use: 1/1)
[Proc 4 (pid 8)]: Requesting Disk (second resource)...
[Proc 4 (pid 8)]: GRANTED Disk (In use: 1/2)
[Proc 4 (pid 8)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 1/3)...
[Proc 2 (pid 6)]: GRANTED Printer (In use: 2/2)
[Proc 2 (pid 6)]: Requesting Disk (second resource)...
[Proc 2 (pid 6)]: GRANTED Disk (In use: 2/2)
[Proc 2 (pid 6)]: Holding [Printer, Disk]. STARTING WORK (Cycle 2/3)...
[Proc 3 (pid 7)]: Requesting Printer (first resource)...
[Proc 2 (pid 6)]: FINISHED WORK. Released [Printer, Disk] (Cycle 2/3 done).
[Proc 3 (pid 7)]: GRANTED Printer (In use: 2/2)
[Proc 3 (pid 7)]: Requesting Scanner (second resource)...
[Proc 4 (pid 8)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 1/3 done).
[Proc 0 (pid 4)]: GRANTED Scanner (In use: 1/1)
[Proc 0 (pid 4)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 2/3)...
[Proc 2 (pid 6)]: Requesting Printer (first resource)...
[Proc 4 (pid 8)]: Requesting Scanner (first resource)...
[Proc 0 (pid 4)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 2/3 done).
[Proc 1 (pid 5)]: GRANTED Scanner (In use: 1/1)
[Proc 1 (pid 5)]: Requesting Disk (second resource)...
[Proc 1 (pid 5)]: GRANTED Disk (In use: 1/2)
[Proc 1 (pid 5)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 2/3)...
[Proc 2 (pid 6)]: GRANTED Printer (In use: 2/2)
[Proc 2 (pid 6)]: Requesting Disk (second resource)...
[Proc 2 (pid 6)]: GRANTED Disk (In use: 2/2)
[Proc 2 (pid 6)]: Holding [Printer, Disk]. STARTING WORK (Cycle 3/3)...
[Proc 0 (pid 4)]: Requesting Printer (first resource)...
[Proc 1 (pid 5)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 2/3 done).
[Proc 2 (pid 6)]: FINISHED WORK. Released [Printer, Disk] (Cycle 3/3 done).
[Proc 3 (pid 7)]: GRANTED Scanner (In use: 1/1)
[Proc 3 (pid 7)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 2/3)...
[Proc 0 (pid 4)]: GRANTED Printer (In use: 2/2)
[Proc 0 (pid 4)]: Requesting Scanner (second resource)...
[Proc 1 (pid 5)]: Requesting Scanner (first resource)...
[Proc 2 (pid 6)]: Completed all 3 cycles cleanly.
[Proc 3 (pid 7)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 2/3 done).
[Proc 4 (pid 8)]: GRANTED Scanner (In use: 1/1)
[Proc 4 (pid 8)]: Requesting Disk (second resource)...
[Proc 4 (pid 8)]: GRANTED Disk (In use: 1/2)
[Proc 4 (pid 8)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 2/3)...
[Proc 3 (pid 7)]: Requesting Printer (first resource)...
[Proc 3 (pid 7)]: GRANTED Printer (In use: 2/2)
[Proc 3 (pid 7)]: Requesting Scanner (second resource)...
[Proc 4 (pid 8)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 2/3 done).
[Proc 0 (pid 4)]: GRANTED Scanner (In use: 1/1)
[Proc 0 (pid 4)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 3/3)...
[Proc 4 (pid 8)]: Requesting Scanner (first resource)...
[Proc 0 (pid 4)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 3/3 done).
[Proc 1 (pid 5)]: GRANTED Scanner (In use: 1/1)
[Proc 1 (pid 5)]: Requesting Disk (second resource)...
[Proc 1 (pid 5)]: GRANTED Disk (In use: 1/2)
[Proc 1 (pid 5)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 3/3)...
[Proc 0 (pid 4)]: Completed all 3 cycles cleanly.
[Proc 1 (pid 5)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 3/3 done).
[Proc 3 (pid 7)]: GRANTED Scanner (In use: 1/1)
[Proc 3 (pid 7)]: Holding [Printer, Scanner]. STARTING WORK (Cycle 3/3)...
[Proc 1 (pid 5)]: Completed all 3 cycles cleanly.
[Proc 3 (pid 7)]: FINISHED WORK. Released [Printer, Scanner] (Cycle 3/3 done).
[Proc 4 (pid 8)]: GRANTED Scanner (In use: 1/1)
[Proc 4 (pid 8)]: Requesting Disk (second resource)...
[Proc 4 (pid 8)]: GRANTED Disk (In use: 1/2)
[Proc 4 (pid 8)]: Holding [Scanner, Disk]. STARTING WORK (Cycle 3/3)...
[Proc 3 (pid 7)]: Completed all 3 cycles cleanly.
[Proc 4 (pid 8)]: FINISHED WORK. Released [Scanner, Disk] (Cycle 3/3 done).
[Proc 4 (pid 8)]: Completed all 3 cycles cleanly.
============================================================
Simulation Finished Summary:
Process 0 completed: 3 / 3 tasks.
Process 1 completed: 3 / 3 tasks.
Process 2 completed: 3 / 3 tasks.
Process 3 completed: 3 / 3 tasks.
Process 4 completed: 3 / 3 tasks.
Total tasks completed: 15 (Expected: 15)
Capacity violations: 0
SUCCESS:
  1. All 5 processes completed without any deadlock or starvation.
  2. Resource capacities (Printer: 2, Scanner: 1, Disk: 2) were strictly preserved.
  3. Resource ordering guaranteed acyclic wait-for relationships.
============================================================
```

---

## Lab 7: Kernel Threads, Sync, Deadlocks & Priority

### `threads` — Kernel Lightweight Threads (clone & join)
**Command**: `make -C xv6-riscv/threads qemu` then `$ threadtest`

```text
$ threadtest
============================================================
   Kernel-Supported Threads Demonstration with clone()      
   Threads: 3, Increments per thread: 100000                   
   Expected counter without race condition: 300000              
============================================================
Main (pid 3): Cloned Thread 1 with pid 4
Main (pid 3): Cloned Thread 2 with pid 5
Main (pid 3): Cloned Thread 3 with pid 6
Main: Releasing all 3 threads concurrently...
Thread 3 (pid 6) TThrheadrfe ia2ndis he( p1di (d 1p0i 5)d 0 fin000 i4incsrehem)d en10 ftsi0.0
0ni0 shedMai 1i0nc0n:r Reape0e00d im thnerecad withr pid 6e vmeniats j.n
tso.in
()
Main: Reaped thread with pid 4 via join()
Main: Reaped thread with pid 5 via join()
============================================================
Simulation Finished Summary:
All 3 threads executed in the SAME shared address space.
Expected Counter : 300000
Actual Counter   : 105493
Race condition confirmed: Lost updates = 194507 (64% loss).
Explanation: Unsynchronized concurrent execution caused
interleaved read-modify-write cycles. (Fixed in Question 2 using Semaphores).
============================================================
```

### `sync` — Process/Thread Sync using Semaphores
**Command**: `make -C xv6-riscv/sync qemu` then `$ synctest`

```text
$ synctest
============================================================
      xv6 Process/Thread Synchronization using Semaphores   
============================================================
============================================================
 PART 1: Synchronized Threads Counter Test using Semaphores 
 Threads: 3, Increments: 100000 each, Expected: 300000            
============================================================
Main: Cloned Thread 1 (pid 4)
Main: Cloned Thread 2 (pid 5)
Main: Cloned Thread 3 (pid 6)
Main: Releasing all 3 threads concurrently...
[Thread 1 (pid 4)] Progress: 25000 / 100000 increments
[Thread 2 (pid 5)] Progress: 25000 / 100000 increments
[Thread 3 (pid 6)] Progress: 25000 / 100000 increments
[Thread 2 (pid 5)] Progress: 50000 / 100000 increments
[Thread 1 (pid 4)] Progress: 50000 / 100000 increments
[Thread 3 (pid 6)] Progress: 50000 / 100000 increments
[Thread 2 (pid 5)] Progress: 75000 / 100000 increments
[Thread 1 (pid 4)] Progress: 75000 / 100000 increments
[Thread 3 (pid 6)] Progress: 75000 / 100000 increments
[Thread 1 (pid 4)] Progress: 100000 / 100000 incre[menThrtse
aThd 2 (preid ad5) 1] P (pidro 4)g finished re100000 ss: sy10000n0 / 1000chroniz00ed  iincrncementres.
mentsMain
T: Rheapreedad 2 thr (peid 5) ad finwitishh ed pid100 4 via jo0in(00)
 synchronized increments.
Main: Reaped thread with pid 5 via join()
[Thread 3 (pid 6)] Progress: 100000 / 100000 increments
Thread 3 (pid 6) finished 100000 synchronized increments.
Main: Reaped thread with pid 6 via join()
------------------------------------------------------------
Part 1 Results: Expected = 300000, Actual = 300000
SUCCESS: Counter reached EXACTLY 300000 with 0 lost updates!
Semaphores successfully eliminated the race condition.

============================================================
 PART 2: Classic Producer-Consumer Bounded Buffer Test      
 Buffer Size = 5, Total Items = 15                         
 Semaphores: Empty (init 5), Full (init 0), Mutex (init 1)  
============================================================
[Producer (pid 7)]: Produced 1 | Buffer count: 1/5
[Consumer (pid 8)]: Consumed 1 | Buffer count: 0/5
[Producer (pid 7)]: Produced 2 | Buffer count: 1/5
[Consumer (pid 8)]: Consumed 2 | Buffer count: 0/5
[Producer (pid 7)]: Produced 3 | Buffer count: 1/5
[Producer (pid 7)]: Produced 4 | Buffer count: 2/5
[Producer (pid 7)]: Produced 5 | Buffer count: 3/5
[Consumer (pid 8)]: Consumed 3 | Buffer count: 2/5
[Producer (pid 7)]: Produced 6 | Buffer count: 3/5
[Producer (pid 7)]: Produced 7 | Buffer count: 4/5
[Consumer (pid 8)]: Consumed 4 | Buffer count: 3/5
[Producer (pid 7)]: Produced 8 | Buffer count: 4/5
[Producer (pid 7)]: Produced 9 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 5 | Buffer count: 4/5
[Producer (pid 7)]: Produced 10 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 6 | Buffer count: 4/5
[Producer (pid 7)]: Produced 11 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 7 | Buffer count: 4/5
[Producer (pid 7)]: Produced 12 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 8 | Buffer count: 4/5
[Producer (pid 7)]: Produced 13 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 9 | Buffer count: 4/5
[Producer (pid 7)]: Produced 14 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 10 | Buffer count: 4/5
[Producer (pid 7)]: Produced 15 | Buffer count: 5/5
[Consumer (pid 8)]: Consumed 11 | Buffer count: 4/5
[Consumer (pid 8)]: Consumed 12 | Buffer count: 3/5
[Consumer (pid 8)]: Consumed 13 | Buffer count: 2/5
[Consumer (pid 8)]: Consumed 14 | Buffer count: 1/5
[Consumer (pid 8)]: Consumed 15 | Buffer count: 0/5
Consumer: All 15 items verified in strict FIFO order!
------------------------------------------------------------
Part 2 Results: Peak Buffer Usage = 5/5 slots, Final Count = 0
SUCCESS: No buffer overflow or underflow occurred.
Producer-Consumer synchronization fully verified!

============================================================
All synchronization tests completed successfully!
============================================================
```

### `deadlock` — Comprehensive Deadlock Detection & Avoidance Suite
**Command**: `make -C xv6-riscv/deadlock qemu` then `$ deadlock`

```text
$ deadlock
============================================================
       Deadlock Detection & Avoidance Algorithms in xv6     
============================================================

============================================================
 TEST MATRIX 1: Deadlock Detection Algorithm (Acyclic / Safe)
 5 Processes (P0..P4), 3 Resource Types (R0..R2)
============================================================

--- System Resource State ---
Proc | Allocation | Request   
     | R0 R1 R2 | R0 R1 R2 
-----+----------+----------
 P0  |  0  1  0 |  0  0  0 
 P1  |  2  0  0 |  2  0  2 
 P2  |  3  0  3 |  0  0  0 
 P3  |  2  1  1 |  1  0  0 
 P4  |  0  0  2 |  0  0  2 
Available: R0=0 R1=0 R2=0 

RESULT: NO DEADLOCK DETECTED.
All processes can eventually terminate and release resources.

============================================================
 TEST MATRIX 2: Deadlock Detection Algorithm (Deadlocked State)
 P2 requests [0, 0, 1] creating circular wait
============================================================

--- System Resource State ---
Proc | Allocation | Request   
     | R0 R1 R2 | R0 R1 R2 
-----+----------+----------
 P0  |  0  1  0 |  0  0  0 
 P1  |  2  0  0 |  2  0  2 
 P2  |  3  0  3 |  0  0  1 
 P3  |  2  1  1 |  1  0  0 
 P4  |  0  0  2 |  0  0  2 
Available: R0=0 R1=0 R2=0 

RESULT: DEADLOCK DETECTED!
Deadlocked processes (4): [ P1 P2 P3 P4 ]
Processes cannot proceed due to circular resource dependencies.

============================================================
 TEST MATRIX 3: Banker's Algorithm Deadlock Avoidance
 Testing initial safety & hypothetical request leading to UNSAFE
============================================================

--- System Resource State ---
Proc | Allocation | Need      
     | R0 R1 R2 | R0 R1 R2 
-----+----------+----------
 P0  |  0  1  0 |  7  4  3 
 P1  |  2  0  0 |  1  2  2 
 P2  |  3  0  2 |  6  0  0 
 P3  |  2  1  1 |  0  1  1 
 P4  |  0  0  2 |  4  3  1 
Available: R0=3 R1=3 R2=2 

Initial State Check: System is in a SAFE state.
Safe sequence: <P1, P3, P4, P0, P2>

--- Request Scenario A ---
Hypothetical Request: Process P1 requests [R0=1, R1=0, R2=2]...
Request Evaluation: GRANTED (System remains in safe state).
New Safe Sequence: <P1, P3, P4, P0, P2>

--- Request Scenario B (Unsafe Request) ---
Hypothetical Request: Process P0 requests [R0=0, R1=3, R2=0]...

--- System Resource State ---
Proc | Allocation | Need      
     | R0 R1 R2 | R0 R1 R2 
-----+----------+----------
 P0  |  0  4  0 |  7  1  3 
 P1  |  2  0  0 |  1  2  2 
 P2  |  3  0  2 |  6  0  0 
 P3  |  2  1  1 |  0  1  1 
 P4  |  0  0  2 |  4  3  1 
Available: R0=3 R1=0 R2=2 

Request Evaluation: DENIED! Would lead to an UNSAFE state.
State successfully rolled back to safe configuration.

--- Request Scenario C (Exceeds Available Resources) ---
Hypothetical Request: Process P0 requests [R0=4, R1=0, R2=0]...
Error: Process must wait, since resources are not available (Requested 4 > Available 3)!
Request Evaluation: REJECTED immediately (insufficient available resources).

============================================================
 Task (c): Real Process Pipe Deadlock Simulation & Detection
 Process A holds Pipe1 (write) and waits to read Pipe2
 Process B holds Pipe2 (write) and waits to read Pipe1
============================================================

--- System Resource State ---
Proc | Allocation | Request   
     | R0 R1 | R0 R1 
-----+-------+-------
 P0  |  1  0 |  0  1 
 P1  |  0  1 |  1  0 
Available: R0=0 R1=0 

ALGORITHM RESULT: DEADLOCK DETECTED among 2 processes!
Deadlocked processes: Process A (P0), Process B (P1)
Explanation: Process A holds Pipe 1 waiting for Pipe 2;
             Process B holds Pipe 2 waiting for Pipe 1 (Circular Wait).

============================================================
All Deadlock Detection and Avoidance tests completed.
============================================================
```

### `priority` — Priority Preemptive Scheduler with Aging
**Command**: `make -C xv6-riscv/priority qemu` then `$ priotest`

```text
$ priotest
============================================================
 Priority Preemptive Scheduler with Dynamic Aging Test     
 Priorities: 1 = Highest, 20 = Lowest. Default = 10         
 Aging Rule: After > 30 ticks waiting, Priority decreases by 1
============================================================
Parent PID: 3 at tick 2

Forking 4 processes in REVERSE priority order:
  Child 1: Forked 1st -> Priority 20 (Lowest)
  Child 2: Forked 2nd -> Priority 12 (Medium-Low)
  Child 3: Forked 3rd -> Priority 8  (Medium-High)
  Child 4: Forked 4th -> Priority 5  (Highest)

Under FCFS: Execution order would be Child 1 -> 2 -> 3 -> 4.
Under Priority Preemptive: Child 4 (Prio 5) MUST run FIRST!

[Parent] Forked Child 1 (PID 4) assigned Priority 20
[Parent] Forked Child 2 (PID 5) assigned Priority 12
[Parent] Forked Child 3 (PID 6) assigned Priority 8
[Parent] Forked Child 4 (PID 7) assigned Priority 5

[Parent] All 4 children forked. Monitoring execution & aging...

[Child 4 | PID 7] Started at tick 2 (Initial Prio: 5, Effective Prio: 5)
[Kernel Aging] PID 4 waited > 30 ticks: Priority boosted 20 -> 19
[Kernel Aging] PID 5 waited > 30 ticks: Priority boosted 12 -> 11
[Kernel Aging] PID 6 waited > 30 ticks: Priority boosted 8 -> 7
[Child 4 | PID 7] COMPLETED at tick 37 (Initial: 5 -> Final: 5)
[Child 3 | PID 6] Started at tick 37 (Initial Prio: 8, Effective Prio: 7)
[Kernel Aging] PID 4 waited > 30 ticks: Priority boosted 19 -> 18
[Kernel Aging] PID 5 waited > 30 ticks: Priority boosted 11 -> 10
[Kernel Aging] PID 3 waited > 30 ticks: Priority boosted 10 -> 9
[Child 3 | PID 6] COMPLETED at tick 72 (Initial: 8 -> Final: 7)

>> Completion Order 1: PID 7 (Child 4, Init Prio: 5, Final Prio: 5, Ran: tick 2 to 37) [Unchanged]

>> Completion Order 2: PID 6 (Child 3, Init Prio: 8, Final Prio: 7, Ran: tick 37 to 72) [AGED UP BY 1 LEVELS!]
[Child 2 | PID 5] Started at tick 72 (Initial Prio: 12, Effective Prio: 10)
[Child 2 | PID 5] COMPLETED at tick 92 (Initial: 12 -> Final: 10)

>> Completion Order 3: PID 5 (Child 2, Init Prio: 12, Final Prio: 10, Ran: tick 72 to 92) [AGED UP BY 2 LEVELS!]
[Child 1 | PID 4] Started at tick 92 (Initial Prio: 20, Effective Prio: 18)
[Child 1 | PID 4] COMPLETED at tick 107 (Initial: 20 -> Final: 18)

>> Completion Order 4: PID 4 (Child 1, Init Prio: 20, Final Prio: 18, Ran: tick 92 to 107) [AGED UP BY 2 LEVELS!]

============================================================
Test Summary & Proof:
1. Priority Preemption Verified (NOT FCFS):
   - Child 4 (PID 7, Prio 5) was forked LAST, but completed FIRST!
   - Child 1 (PID 4, Prio 20) was forked FIRST, but completed LAST!
   - The completion order is the exact REVERSE of fork order, proving priority scheduling.

2. Anti-Starvation Dynamic Aging Verified:
   - Real-time kernel aging logs confirmed wait-tick thresholds (> 30 ticks).
   - Child 3 (PID 6): Waited while higher priorities ran; aged from 8 -> 7!
   - Child 2 (PID 5): Waited while higher priorities ran; aged from 12 -> 10!
   - Child 1 (PID 4): Waited while higher priorities ran; aged from 20 -> 18!
   - Lowest priority process (Initial Prio 20) was never starved and finished cleanly.
============================================================
```

---

## Lab 8: Virtual Memory & Page Table Management

### `va2pa` — Virtual to Physical Address Translation
**Command**: `make -C xv6-riscv/va2pa qemu` then `$ vatest`

```text
$ vatest
============================================================
  Virtual to Physical Address Translation Test (vatest)     
============================================================

--- Step 1: Translate Global, Local (Stack), and Heap Variables ---
[cprintf/kernel] va2pa: VA 0x2000 -> PDE_idx:[0,0], PTE_idx:2, Offset:0x0, Flags:[P=1, W=1, U=1] -> PA:0x87f1e000
Global Variable:  VA = 0x2000  -->  PA = 0x87F1E000
[cprintf/kernel] va2pa: VA 0x4fac -> PDE_idx:[0,0], PTE_idx:4, Offset:0xfac, Flags:[P=1, W=1, U=1] -> PA:0x87f1cfac
Local Variable:   VA = 0x4FAC  -->  PA = 0x87F1CFAC
[cprintf/kernel] va2pa: VA 0x14f80 -> PDE_idx:[0,0], PTE_idx:20, Offset:0xf80, Flags:[P=1, W=1, U=1] -> PA:0x87f30f80
Heap Variable:    VA = 0x14F80  -->  PA = 0x87F30F80

--- Step 2: Unmapped Address Translation (0x7FFFFFFF) ---
Unmapped Address: VA = 0x7FFFFFFF  -->  Result = -1 (Expected: -1)
SUCCESS: va2pa correctly returned -1 for unmapped address.

--- Step 3: Forking Child to Demonstrate Distinct Physical Pages ---
[cprintf/kernel] va2pa: VA 0x2000 -> PDE_idx:[0,0], PTE_idx:2, Offset:0x0, Flags:[P=1, W=1, U=1] -> PA:0x87f45000
[Child PID 4]  VA = 0x2000  -->  PA = 0x87F45000
[cprintf/kernel] va2pa: VA 0x2000 -> PDE_idx:[0,0], PTE_idx:2, Offset:0x0, Flags:[P=1, W=1, U=1] -> PA:0x87f1e000
[Parent PID 3] VA = 0x2000  -->  PA = 0x87F1E000

VERIFICATION:
Same virtual address:   0x2000 (Parent & Child)
Separate physical page: PA copied on fork (Parent PA != Child PA)

============================================================
vatest passed all verification steps successfully!
============================================================
```

### `meminfo` — Physical Memory Statistics & Leak Check
**Command**: `make -C xv6-riscv/meminfo qemu` then `$ memtest`

```text
$ memtest
============================================================
         Physical Memory Statistics Test (memtest)          
============================================================

[1. Initial Baseline]
  Total System Pages:      32732 (130928 KB)
  Free Physical Pages:     32531 (130124 KB)
  Process Address Pages:   5 (20 KB, sz/PGSIZE)
  Process Page-Table Pages:5 (20 KB)

--- Step 2: Allocate 4 Pages (16 KB) with sbrk(4 * 4096) ---
[2. After Heap Expansion (sbrk +4 pages)]
  Total System Pages:      32732 (130928 KB)
  Free Physical Pages:     32527 (130108 KB)
  Process Address Pages:   9 (36 KB, sz/PGSIZE)
  Process Page-Table Pages:5 (20 KB)
  -> Free pages change: 32531 -> 32527 (decreased by 4)
  -> Process user pages: 5 -> 9 (increased by 4)

--- Step 3: Forking a Child Process ---
[3a. Inside Child Process]
  Total System Pages:      32732 (130928 KB)
  Free Physical Pages:     32512 (130048 KB)
  Process Address Pages:   9 (36 KB, sz/PGSIZE)
  Process Page-Table Pages:5 (20 KB)
  -> Free pages dropped further due to child address space and page tables.
[3b. Parent After Reaping Child]
  Total System Pages:      32732 (130928 KB)
  Free Physical Pages:     32527 (130108 KB)
  Process Address Pages:   9 (36 KB, sz/PGSIZE)
  Process Page-Table Pages:5 (20 KB)

--- Verification: Memory Leak Check After Child Termination ---
  Free pages before fork: 32527
  Free pages after reap:  32527
  >> SUCCESS: Exact match! All child frames and page tables reaped (NO LEAK).

--- Step 4: Shrink Heap with sbrk(-4 * 4096) ---
[4. After Heap Shrinking]
  Total System Pages:      32732 (130928 KB)
  Free Physical Pages:     32531 (130124 KB)
  Process Address Pages:   5 (20 KB, sz/PGSIZE)
  Process Page-Table Pages:5 (20 KB)
  -> Free pages returned from 32527 to 32531 (restored 4 pages)

============================================================
memtest completed all tasks successfully!
============================================================
```

### `vmprint` — Process Page Table Hierarchical Dump
**Command**: `make -C xv6-riscv/vmprint qemu` then `$ vmprinttest`

```text
$ vmprinttest
============================================================
          Process Page Table Dump Test (vmprint)            
============================================================

------------------------------------------------------------
Phase 1: Initial Page Table Dump at Process Startup
------------------------------------------------------------
page table 0x0000000087f25000 (pid 3, sz 20480):
..dir[0]: pa 0x87f21000, flags: [P=1, W=0, U=0]
  ..mid[0]: pa 0x87f20000, flags: [P=1, W=0, U=0]
    ..pte[0]: va 0x0 -> pa 0x87f22000, flags: [P=1, W=0, U=1]
    ..pte[1]: va 0x1000 -> pa 0x87f1f000, flags: [P=1, W=0, U=1]
    ..pte[2]: va 0x2000 -> pa 0x87f1e000, flags: [P=1, W=1, U=1]
    ..pte[3]: va 0x3000 -> pa 0x87f1d000, flags: [P=1, W=1, U=0] <-- [STACK GUARD PAGE: PTE_U=0]
    ..pte[4]: va 0x4000 -> pa 0x87f1c000, flags: [P=1, W=1, U=1]
..dir[255]: pa 0x87f24000, flags: [P=1, W=0, U=0]
  ..mid[511]: pa 0x87f23000, flags: [P=1, W=0, U=0]
Kernel entries summary: 2 high-memory kernel entries mapped (trampoline/trapframe).
User accessible pages: 4 (sz/PGSIZE=5 minus guard page=1)

------------------------------------------------------------
Phase 2: Growing Heap with sbrk(8 * 4096)
------------------------------------------------------------
Allocated 8 new pages (32 KB) in heap.
page table 0x0000000087f25000 (pid 3, sz 53248):
..dir[0]: pa 0x87f21000, flags: [P=1, W=0, U=0]
  ..mid[0]: pa 0x87f20000, flags: [P=1, W=0, U=0]
    ..pte[0]: va 0x0 -> pa 0x87f22000, flags: [P=1, W=0, U=1]
    ..pte[1]: va 0x1000 -> pa 0x87f1f000, flags: [P=1, W=0, U=1]
    ..pte[2]: va 0x2000 -> pa 0x87f1e000, flags: [P=1, W=1, U=1]
    ..pte[3]: va 0x3000 -> pa 0x87f1d000, flags: [P=1, W=1, U=0] <-- [STACK GUARD PAGE: PTE_U=0]
    ..pte[4]: va 0x4000 -> pa 0x87f1c000, flags: [P=1, W=1, U=1]
    ..pte[5]: va 0x5000 -> pa 0x87f26000, flags: [P=1, W=1, U=1]
    ..pte[6]: va 0x6000 -> pa 0x87f54000, flags: [P=1, W=1, U=1]
    ..pte[7]: va 0x7000 -> pa 0x87f53000, flags: [P=1, W=1, U=1]
    ..pte[8]: va 0x8000 -> pa 0x87f49000, flags: [P=1, W=1, U=1]
    ..pte[9]: va 0x9000 -> pa 0x87f46000, flags: [P=1, W=1, U=1]
    ..pte[10]: va 0xa000 -> pa 0x87f43000, flags: [P=1, W=1, U=1]
    ..pte[11]: va 0xb000 -> pa 0x87f27000, flags: [P=1, W=1, U=1]
    ..pte[12]: va 0xc000 -> pa 0x87f28000, flags: [P=1, W=1, U=1]
..dir[255]: pa 0x87f24000, flags: [P=1, W=0, U=0]
  ..mid[511]: pa 0x87f23000, flags: [P=1, W=0, U=0]
Kernel entries summary: 2 high-memory kernel entries mapped (trampoline/trapframe).
User accessible pages: 12 (sz/PGSIZE=13 minus guard page=1)

------------------------------------------------------------
Phase 3: Shrinking Heap with sbrk(-4096)
------------------------------------------------------------
Deallocated 1 page (4 KB) from heap.
page table 0x0000000087f25000 (pid 3, sz 49152):
..dir[0]: pa 0x87f21000, flags: [P=1, W=0, U=0]
  ..mid[0]: pa 0x87f20000, flags: [P=1, W=0, U=0]
    ..pte[0]: va 0x0 -> pa 0x87f22000, flags: [P=1, W=0, U=1]
    ..pte[1]: va 0x1000 -> pa 0x87f1f000, flags: [P=1, W=0, U=1]
    ..pte[2]: va 0x2000 -> pa 0x87f1e000, flags: [P=1, W=1, U=1]
    ..pte[3]: va 0x3000 -> pa 0x87f1d000, flags: [P=1, W=1, U=0] <-- [STACK GUARD PAGE: PTE_U=0]
    ..pte[4]: va 0x4000 -> pa 0x87f1c000, flags: [P=1, W=1, U=1]
    ..pte[5]: va 0x5000 -> pa 0x87f26000, flags: [P=1, W=1, U=1]
    ..pte[6]: va 0x6000 -> pa 0x87f54000, flags: [P=1, W=1, U=1]
    ..pte[7]: va 0x7000 -> pa 0x87f53000, flags: [P=1, W=1, U=1]
    ..pte[8]: va 0x8000 -> pa 0x87f49000, flags: [P=1, W=1, U=1]
    ..pte[9]: va 0x9000 -> pa 0x87f46000, flags: [P=1, W=1, U=1]
    ..pte[10]: va 0xa000 -> pa 0x87f43000, flags: [P=1, W=1, U=1]
    ..pte[11]: va 0xb000 -> pa 0x87f27000, flags: [P=1, W=1, U=1]
..dir[255]: pa 0x87f24000, flags: [P=1, W=0, U=0]
  ..mid[511]: pa 0x87f23000, flags: [P=1, W=0, U=0]
Kernel entries summary: 2 high-memory kernel entries mapped (trampoline/trapframe).
User accessible pages: 11 (sz/PGSIZE=12 minus guard page=1)

============================================================
Verification Summary:
1. Hierarchical two/three-level page table structure clearly dumped.
2. Stack guard page identified with PTE_U=0.
3. User-accessible pages count matches sz/PGSIZE minus guard page.
4. Heap expansion (+8 pages) and heap shrink (-1 page) verified.
============================================================
```

### `lazy` — Lazy Memory Allocation (Demand Paging)
**Command**: `make -C xv6-riscv/lazy qemu` then `$ lazytest`

```text
$ lazytest
============================================================
      Lazy Memory Allocation (Demand Paging) Test           
============================================================

--- Step 1: Initial Baseline Memory ---
  Free physical pages:     32531 (130124 KB)
  Process user pages:      5

--- Step 2: Request Large sbrk(32 MB) ---
  sbrk returned base address: 0x0000000000005000
  Process virtual pages (sz/PGSIZE): 5 -> 8197 (+8192 pages)
  Free physical pages: 32531 -> 32531 (change = 0 pages)
  >> SUCCESS: 32 MB allocated lazily without consuming physical frames!

--- Step 3: Touching 3 Widely Spaced Pages (0 MB, 16 MB, 31 MB) ---
  Read back touched bytes: p[0]=A, p[16MB]=B, p[31MB]=C
  Free physical pages: 32531 -> 32526 (consumed 5 physical frames for 3 pages + page tables)
  >> SUCCESS: Only touched pages triggered on-demand page faults!

--- Step 4: Kernel System Call (read/pipe) into Untouched Heap Buffer ---
  Pipe read into untouched buffer returned: 12 bytes, content: 'HELLO_LAZY!'
  >> SUCCESS: Kernel successfully handled demand paging during syscall!

--- Step 5: Testing fork() & exit() with Lazy Mappings ---
  Child process executed, accessed lazy pages, and exited cleanly.

--- Step 6: Testing Invalid Access Beyond sz (Expect Child Process Termination) ---
  [Child] Attempting invalid write to p[33 MB]...
usertrap(): page fault at invalid va 0x2105000 (sz 0x2005000, sp 0x4f20) pid=5
  Parent successfully reaped faulted child. Kernel is intact and running!

============================================================
lazytest passed all tasks and edge-case verifications!
============================================================
```

---

## Lab 9: Page Table Printing, Faults & Replacement (FIFO & LRU)

### `ptprint` — Page Table Printing (vmprint)
**Command**: `make -C xv6-riscv/ptprint qemu` then `$ pttest`

```text
$ pttest
============================================================
         Page Table Printing Test Program (pttest)          
============================================================

--- 1. Calling vmprint() at initial start-up ---
=== Page Table Dump for PID 3 (pttest) ===
  Virtual Addr       Physical Addr      Flags [P, W, U]
  -----------------------------------------------------
  0x0  ->  0x87f22000    [P: 1, W: 0, U: 1]
  0x1000  ->  0x87f1f000    [P: 1, W: 0, U: 1]
  0x2000  ->  0x87f1e000    [P: 1, W: 1, U: 1]
  0x4000  ->  0x87f1c000    [P: 1, W: 1, U: 1]
  -----------------------------------------------------
  Total valid user pages : 4
  Total page-table pages : 5
=====================================================
Initial valid user pages reported by vmprint: 4

--- 2. Allocating 4 pages via sbrk(4 * 4096 = 16384 bytes) ---
sbrk allocated memory starting at: 0x5000
Touched all 4 newly allocated pages.

--- 3. Calling vmprint() after sbrk(4 * 4096) ---
=== Page Table Dump for PID 3 (pttest) ===
  Virtual Addr       Physical Addr      Flags [P, W, U]
  -----------------------------------------------------
  0x0  ->  0x87f22000    [P: 1, W: 0, U: 1]
  0x1000  ->  0x87f1f000    [P: 1, W: 0, U: 1]
  0x2000  ->  0x87f1e000    [P: 1, W: 1, U: 1]
  0x4000  ->  0x87f1c000    [P: 1, W: 1, U: 1]
  0x5000  ->  0x87f26000    [P: 1, W: 1, U: 1]
  0x6000  ->  0x87f54000    [P: 1, W: 1, U: 1]
  0x7000  ->  0x87f53000    [P: 1, W: 1, U: 1]
  0x8000  ->  0x87f49000    [P: 1, W: 1, U: 1]
  -----------------------------------------------------
  Total valid user pages : 8
  Total page-table pages : 5
=====================================================
New valid user pages reported by vmprint: 8

--- 4. Verification Analysis ---
Initial user pages : 4
After sbrk(+4)     : 8
Difference         : 4 pages
SUCCESS: Exactly four new pages appeared in the page table!
============================================================
```

### `lazyfault` — Demand Paging with Page Fault Counter
**Command**: `make -C xv6-riscv/lazyfault qemu` then `$ faulttest`

```text
$ faulttest
============================================================
        Lazy Memory Allocation Test (faulttest)             
============================================================

--- 1. Baseline Page Fault Count ---
Initial handled page faults: 2

--- 2. Requesting 32 pages via sbrk(32 * 4096 = 131072 bytes) ---
sbrk returned base address: 0x5000
Handled page faults immediately after sbrk: 2
PASSED: No physical pages allocated yet (lazy allocation active!)

--- 3. Writing to exactly 5 distinct pages ---
  Touched page index 0 (VA 0x5000) with 'A'
  Touched page index 4 (VA 0x9000) with 'B'
  Touched page index 11 (VA 0x10000) with 'C'
  Touched page index 19 (VA 0x18000) with 'D'
  Touched page index 31 (VA 0x24000) with 'E'

--- 4. Checking Page Fault Count via getfaults() ---
Total handled page faults: 7 (New faults: 5)
SUCCESS: Exactly 5 page faults were handled by the kernel!

--- 5. Testing Access Above Allocated Memory Size ---
[Child pid 4] Attempting to access address above p->sz (VA 0x28000)...
Page fault: pid=4 address=0x28000 exceeds size=0x25000 (killed)
[Parent] Child process terminated with status -1.
[Parent] SUCCESS: Only the offending child was killed by the kernel!

============================================================
faulttest: All lazy allocation tests completed successfully!
============================================================
```

### `pagerepl` — Page Replacement Simulation (FIFO vs LRU)
**Command**: `make -C xv6-riscv/pagerepl qemu` then `$ pagerepl`

```text
$ pagerepl
============================================================
     Page Replacement Algorithms Simulation (FIFO vs LRU)  
============================================================

Reference String (20 references):
  7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1 

============================================================
TASK B: Testing with 3 Frames (Step-by-Step Trace)
============================================================

------------------------------------------------------------
 FIFO Simulation (Frames: 3)
------------------------------------------------------------
  Step	|  Page	|  Frame Contents	| Status
------------------------------------------------------------
  1	|   7	| [ 7  -  - ]	| FAULT
  2	|   0	| [ 7  0  - ]	| FAULT
  3	|   1	| [ 7  0  1 ]	| FAULT
  4	|   2	| [ 2  0  1 ]	| FAULT
  5	|   0	| [ 2  0  1 ]	| HIT 
  6	|   3	| [ 2  3  1 ]	| FAULT
  7	|   0	| [ 2  3  0 ]	| FAULT
  8	|   4	| [ 4  3  0 ]	| FAULT
  9	|   2	| [ 4  2  0 ]	| FAULT
  10	|   3	| [ 4  2  3 ]	| FAULT
  11	|   0	| [ 0  2  3 ]	| FAULT
  12	|   3	| [ 0  2  3 ]	| HIT 
  13	|   2	| [ 0  2  3 ]	| HIT 
  14	|   1	| [ 0  1  3 ]	| FAULT
  15	|   2	| [ 0  1  2 ]	| FAULT
  16	|   0	| [ 0  1  2 ]	| HIT 
  17	|   1	| [ 0  1  2 ]	| HIT 
  18	|   7	| [ 7  1  2 ]	| FAULT
  19	|   0	| [ 7  0  2 ]	| FAULT
  20	|   1	| [ 7  0  1 ]	| FAULT
------------------------------------------------------------
FIFO Result (3 frames): Faults = 15, Hits = 5, Hit Ratio = 25.00%

------------------------------------------------------------
 LRU Simulation (Frames: 3)
------------------------------------------------------------
  Step	|  Page	|  Frame Contents	| Status
------------------------------------------------------------
  1	|   7	| [ 7  -  - ]	| FAULT
  2	|   0	| [ 7  0  - ]	| FAULT
  3	|   1	| [ 7  0  1 ]	| FAULT
  4	|   2	| [ 2  0  1 ]	| FAULT
  5	|   0	| [ 2  0  1 ]	| HIT 
  6	|   3	| [ 2  0  3 ]	| FAULT
  7	|   0	| [ 2  0  3 ]	| HIT 
  8	|   4	| [ 4  0  3 ]	| FAULT
  9	|   2	| [ 4  0  2 ]	| FAULT
  10	|   3	| [ 4  3  2 ]	| FAULT
  11	|   0	| [ 0  3  2 ]	| FAULT
  12	|   3	| [ 0  3  2 ]	| HIT 
  13	|   2	| [ 0  3  2 ]	| HIT 
  14	|   1	| [ 1  3  2 ]	| FAULT
  15	|   2	| [ 1  3  2 ]	| HIT 
  16	|   0	| [ 1  0  2 ]	| FAULT
  17	|   1	| [ 1  0  2 ]	| HIT 
  18	|   7	| [ 1  0  7 ]	| FAULT
  19	|   0	| [ 1  0  7 ]	| HIT 
  20	|   1	| [ 1  0  7 ]	| HIT 
------------------------------------------------------------
LRU Result (3 frames): Faults = 12, Hits = 8, Hit Ratio = 40.00%

--- Verification Against Expected Results (3 Frames) ---
FIFO Faults: 15 (Expected: 15) -> VERIFIED
LRU  Faults: 12 (Expected: 12) -> VERIFIED

============================================================
TASK C: Performance Comparison Across 3, 4, and 5 Frames
============================================================

+-----------+--------+-------------+-----------+-----------+
| Algorithm | Frames | Page Faults | Page Hits | Hit Ratio |
+-----------+--------+-------------+-----------+-----------+
| FIFO      |   3    |     15      |    5     |   25.00%   |
| LRU       |   3    |     12      |    8     |   40.00%   |
+-----------+--------+-------------+-----------+-----------+
| FIFO      |   4    |     10      |    10     |   50.00%   |
| LRU       |   4    |     8      |    12     |   60.00%   |
+-----------+--------+-------------+-----------+-----------+
| FIFO      |   5    |     9      |    11     |   55.00%   |
| LRU       |   5    |     7      |    13     |   65.00%   |
+-----------+--------+-------------+-----------+-----------+

============================================================
pagerepl: Simulation finished successfully!
============================================================
```

---

## Lab 10: Memory Management, Protection & Page Replacement

### `memstats` — Physical Memory Allocation Tracking
**Command**: `make -C xv6-riscv/memstats qemu` then `$ memtest`

```text
$ memtest
============================================================
  Physical Memory Allocation Tracking Test (memtest)        
============================================================

--- Step 1: Baseline Memory Statistics ---

--- Physical Memory Allocation Statistics ---
  Total Allocated Pages:     241 (964 KB)
  Total Freed Pages:         40 (160 KB)
  Currently Allocated Pages: 201 (804 KB)
  Available Physical Pages:  32531 (130124 KB)
---------------------------------------------

--- Step 2: Allocating 10 Pages (40 KB) using sbrk() ---
Memory allocated at virtual address: 0x0000000000005000
Updated statistics after allocation:

--- Physical Memory Allocation Statistics ---
  Total Allocated Pages:     251 (1004 KB)
  Total Freed Pages:         40 (160 KB)
  Currently Allocated Pages: 211 (844 KB)
  Available Physical Pages:  32521 (130084 KB)
---------------------------------------------

--- Step 3: Deallocating 10 Pages using sbrk(-40960) ---
Updated statistics after deallocation:

--- Physical Memory Allocation Statistics ---
  Total Allocated Pages:     251 (1004 KB)
  Total Freed Pages:         50 (200 KB)
  Currently Allocated Pages: 201 (804 KB)
  Available Physical Pages:  32531 (130124 KB)
---------------------------------------------

============================================================
memtest completed successfully!
Observations:
  1. Allocating 10 pages increased total & currently allocated counters
     and decreased available physical memory.
  2. Releasing the memory increased total freed pages and restored
     the available physical pages.
============================================================
```

### `vtop` — Virtual-to-Physical Address Translation (vtop)
**Command**: `make -C xv6-riscv/vtop qemu` then `$ vttest`

```text
$ vttest
============================================================
    Virtual-to-Physical Address Translation Test (vtop)     
============================================================

--- 1. Testing Code and Global Variables ---
[vtop] main() function code:
       VA: 0x4C  ->  PA: 0x87F2204C
[vtop] global_var (global data):
       VA: 0x2000  ->  PA: 0x87F1E000

--- 2. Testing Stack Variable ---
[vtop] stack_var (local stack variable):
       VA: 0x4FAC  ->  PA: 0x87F1CFAC

--- 3. Testing Heap Allocation via sbrk() ---
[vtop] heap_ptr (allocated heap page base):
       VA: 0x5000  ->  PA: 0x87F26000
[vtop] heap_ptr + 256 (offset within page):
       VA: 0x5100  ->  PA: 0x87F26100

--- 4. Testing Unmapped Virtual Addresses ---
[vtop] Unmapped Address 1 (arbitrary high address):
       VA: 0x7FFFFFF0  ->  PA: -1 (UNMAPPED / INVALID)
[vtop] Unmapped Address 2 (far above heap break):
       VA: 0x4005000  ->  PA: -1 (UNMAPPED / INVALID)

============================================================
vttest: All translation tests completed successfully!
============================================================
```

### `pageprotect` — Page Permissions & setreadonly
**Command**: `make -C xv6-riscv/pageprotect qemu` then `$ pagetest`

```text
$ pagetest
============================================================
  Page Table Management & Page Protection Test (pagetest)   
============================================================

--- 1. Allocating a physical page using sbrk(4096) ---
Allocated page at Virtual Address: 0x5000

--- 2. Writing initial data to the allocated page ---
Successfully wrote "HI" to page at 0x5000

--- 3. Checking pageinfo BEFORE setreadonly ---
[pageinfo] VA: 0x5000 -> PFN: 0x87f26 (PA: 0x87f26000) | Present: 1, Writable: 1, User: 1
Expected: Present=1, Writable=1, User=1

--- 4. Invoking setreadonly() to mark page READ-ONLY ---
setreadonly() returned success.

--- 5. Checking pageinfo AFTER setreadonly ---
[pageinfo] VA: 0x5000 -> PFN: 0x87f26 (PA: 0x87f26000) | Present: 1, Writable: 0, User: 1
Expected: Present=1, Writable=0, User=1 (Write permission removed!)

--- 6. Verifying READ operations still succeed ---
Read value from read-only page: 'HI'
Read verification passed!

--- 7. Attempting WRITE to read-only page in a child process ---
[Child pid 4] Attempting to write 'X' to read-only address 0x5000...
usertrap(): unexpected scause 0xf pid=4
            sepc=0x146 stval=0x5000
[Parent] Child process terminated with status -1.
[Parent] SUCCESS: Child process was killed by kernel due to page fault exception!

--- 8. Testing unmapped address error handling ---
Calling pageinfo on unmapped address 0x7FFFFFF0:
[pageinfo] VA: 0x7ffffff0 is NOT MAPPED
pageinfo returned: -1 (expected -1)
Calling setreadonly on unmapped address 0x7FFFFFF0:
setreadonly returned: -1 (expected -1)

============================================================
pagetest: All page protection tests completed successfully!
============================================================
```

### `pagereplace` — Page Replacement (FIFO, LRU, Optimal)
**Command**: `make -C xv6-riscv/pagereplace qemu` then `$ pagereplace`

```text
$ pagereplace
======================================================================
     Page Replacement Algorithms Simulation (FIFO, LRU, Optimal)      
======================================================================

Reference String (20 page requests):
 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1 

======================================================================
PART 1: Step-by-Step Simulation with 3 Physical Frames
======================================================================

------------------------------------------------------------
 FIFO Simulation (Frames: 3)
------------------------------------------------------------
 Step	| Ref	| Frame State	| Status
------------------------------------------------------------
 1	| 7	|  7  -  - 	| FAULT
 2	| 0	|  7  0  - 	| FAULT
 3	| 1	|  7  0  1 	| FAULT
 4	| 2	|  2  0  1 	| FAULT
 5	| 0	|  2  0  1 	| HIT 
 6	| 3	|  2  3  1 	| FAULT
 7	| 0	|  2  3  0 	| FAULT
 8	| 4	|  4  3  0 	| FAULT
 9	| 2	|  4  2  0 	| FAULT
 10	| 3	|  4  2  3 	| FAULT
 11	| 0	|  0  2  3 	| FAULT
 12	| 3	|  0  2  3 	| HIT 
 13	| 2	|  0  2  3 	| HIT 
 14	| 1	|  0  1  3 	| FAULT
 15	| 2	|  0  1  2 	| FAULT
 16	| 0	|  0  1  2 	| HIT 
 17	| 1	|  0  1  2 	| HIT 
 18	| 7	|  7  1  2 	| FAULT
 19	| 0	|  7  0  2 	| FAULT
 20	| 1	|  7  0  1 	| FAULT
------------------------------------------------------------
FIFO Summary (3 frames): Page Faults = 15, Page Hits = 5
Hit Ratio: 25.00% | Fault Ratio: 75.00%

------------------------------------------------------------
 LRU Simulation (Frames: 3)
------------------------------------------------------------
 Step	| Ref	| Frame State	| Status
------------------------------------------------------------
 1	| 7	|  7  -  - 	| FAULT
 2	| 0	|  7  0  - 	| FAULT
 3	| 1	|  7  0  1 	| FAULT
 4	| 2	|  2  0  1 	| FAULT
 5	| 0	|  2  0  1 	| HIT 
 6	| 3	|  2  0  3 	| FAULT
 7	| 0	|  2  0  3 	| HIT 
 8	| 4	|  4  0  3 	| FAULT
 9	| 2	|  4  0  2 	| FAULT
 10	| 3	|  4  3  2 	| FAULT
 11	| 0	|  0  3  2 	| FAULT
 12	| 3	|  0  3  2 	| HIT 
 13	| 2	|  0  3  2 	| HIT 
 14	| 1	|  1  3  2 	| FAULT
 15	| 2	|  1  3  2 	| HIT 
 16	| 0	|  1  0  2 	| FAULT
 17	| 1	|  1  0  2 	| HIT 
 18	| 7	|  1  0  7 	| FAULT
 19	| 0	|  1  0  7 	| HIT 
 20	| 1	|  1  0  7 	| HIT 
------------------------------------------------------------
LRU Summary (3 frames): Page Faults = 12, Page Hits = 8
Hit Ratio: 40.00% | Fault Ratio: 60.00%

------------------------------------------------------------
 Optimal (OPT) Simulation (Frames: 3)
------------------------------------------------------------
 Step	| Ref	| Frame State	| Status
------------------------------------------------------------
 1	| 7	|  7  -  - 	| FAULT
 2	| 0	|  7  0  - 	| FAULT
 3	| 1	|  7  0  1 	| FAULT
 4	| 2	|  2  0  1 	| FAULT
 5	| 0	|  2  0  1 	| HIT 
 6	| 3	|  2  0  3 	| FAULT
 7	| 0	|  2  0  3 	| HIT 
 8	| 4	|  2  4  3 	| FAULT
 9	| 2	|  2  4  3 	| HIT 
 10	| 3	|  2  4  3 	| HIT 
 11	| 0	|  2  0  3 	| FAULT
 12	| 3	|  2  0  3 	| HIT 
 13	| 2	|  2  0  3 	| HIT 
 14	| 1	|  2  0  1 	| FAULT
 15	| 2	|  2  0  1 	| HIT 
 16	| 0	|  2  0  1 	| HIT 
 17	| 1	|  2  0  1 	| HIT 
 18	| 7	|  7  0  1 	| FAULT
 19	| 0	|  7  0  1 	| HIT 
 20	| 1	|  7  0  1 	| HIT 
------------------------------------------------------------
OPT Summary (3 frames): Page Faults = 9, Page Hits = 11
Hit Ratio: 55.00% | Fault Ratio: 45.00%

======================================================================
PART 2: Multi-Frame Performance Comparison (3, 4, and 5 Frames)
======================================================================

+-----------+--------+-------------+-----------+-----------+------------+
| Algorithm | Frames | Page Faults | Page Hits | Hit Ratio | Fault Ratio|
+-----------+--------+-------------+-----------+-----------+------------+
| FIFO      |   3    |     15      |    5     |  25.00%   |   75.00%  |
| LRU       |   3    |     12      |    8     |  40.00%   |   60.00%  |
| OPT       |   3    |     9      |    11     |  55.00%   |   45.00%  |
+-----------+--------+-------------+-----------+-----------+------------+
| FIFO      |   4    |     10      |    10     |  50.00%   |   50.00%  |
| LRU       |   4    |     8      |    12     |  60.00%   |   40.00%  |
| OPT       |   4    |     8      |    12     |  60.00%   |   40.00%  |
+-----------+--------+-------------+-----------+-----------+------------+
| FIFO      |   5    |     9      |    11     |  55.00%   |   45.00%  |
| LRU       |   5    |     7      |    13     |  65.00%   |   35.00%  |
| OPT       |   5    |     7      |    13     |  65.00%   |   35.00%  |
+-----------+--------+-------------+-----------+-----------+------------+

======================================================================
PART 3: Algorithm Performance Analysis and Conclusion
======================================================================
1. Optimal (OPT):
   - Achieved the lowest page faults across all frame sizes:
     9 faults (3 frames), 8 faults (4 frames), and 7 faults (5 frames).
   - Why: OPT looks ahead and replaces the page that will not be used
     for the longest duration in the future. It is provably optimal and
     serves as the theoretical upper bound for hit ratio.

2. Least Recently Used (LRU):
   - Achieved 12 faults (3 frames), 8 faults (4 frames), and 7 faults (5 frames).
   - Why: LRU leverages the Principle of Locality (specifically Temporal
     Locality) by approximating future behavior with past recency.
     At 4 and 5 frames, LRU matched the performance of OPT exactly!

3. First-In, First-Out (FIFO):
   - Suffered the highest page faults:
     15 faults (3 frames), 10 faults (4 frames), and 9 faults (5 frames).
   - Why: FIFO only considers the arrival order without accounting for
     frequency or recency of page accesses, evicting heavily used pages.

Conclusion:
  -> Optimal (OPT) performed best overall in minimizing page faults.
  -> Among implementable online algorithms, LRU significantly outperformed
     FIFO, achieving up to 65.00% hit ratio.
======================================================================
```
