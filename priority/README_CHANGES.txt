================================================================================
Priority-Based Preemptive Scheduler with Aging in xv6
================================================================================

1. OVERVIEW & OBJECTIVES
--------------------------------------------------------------------------------
This implementation replaces the default xv6 round-robin scheduler with a
Priority-Based Preemptive Scheduler with Anti-Starvation Aging:
a) Priorities range from 1 (highest) to 20 (lowest). Default priority is 10.
b) System calls:
   - int set_priority(int pid, int priority): Sets the priority of the target
     process (or current process if pid == 0) and resets its wait counter.
   - int get_priority(int pid): Returns the current effective priority.
c) Preemptive Scheduling:
   scheduler() always selects the RUNNABLE process with the lowest priority
   number (highest priority). Ties are broken using Round-Robin order.
d) Anti-Starvation Aging:
   Every clock tick (clockintr()), wait_ticks is incremented for all RUNNABLE
   processes. If any RUNNABLE process waits for more than 30 ticks without
   running (wait_ticks > 30), its priority number is decremented by 1 (raising
   its priority, bounded at 1), and its wait_ticks is reset to 0. When a process
   is scheduled, its wait_ticks is also reset to 0.

2. FILES MODIFIED & ADDED
--------------------------------------------------------------------------------
- kernel/proc.h:
  * Added int priority (1 to 20, default 10) to struct proc.
  * Added uint wait_ticks to track time spent waiting in RUNNABLE state.
- kernel/proc.c:
  * allocproc(): Initialized p->priority = 10, p->wait_ticks = 0.
  * freeproc(): Reset p->priority = 10, p->wait_ticks = 0.
  * set_priority(pid, new_prio): Validates priority bounds (1..20), sets
    priority, and resets wait_ticks.
  * get_priority(pid): Looks up and returns process priority.
  * update_proc_aging(): Scans proc table, increments wait_ticks for RUNNABLE
    processes, decrements priority number when wait_ticks > 30, and resets wait_ticks.
  * scheduler(): Scans all RUNNABLE processes to find the minimum priority number,
    then round-robin selects the next RUNNABLE process with that minimum priority,
    resets its wait_ticks to 0, and context-switches to it.
- kernel/trap.c:
  * In clockintr() on CPU 0: Calls update_proc_aging() on every timer interrupt.
- kernel/syscall.h & kernel/syscall.c:
  * Registered SYS_set_priority (23) and SYS_get_priority (24).
- kernel/sysproc.c:
  * Added sys_set_priority() and sys_get_priority().
- kernel/defs.h:
  * Declared set_priority, get_priority, and update_proc_aging.
- user/user.h & user/usys.pl:
  * Exported set_priority() and get_priority() to user space.
- Makefile:
  * Set DETFLAGS = empty to support paths containing spaces.
  * Set CPUS := 1 to demonstrate clean single-core scheduling order.
  * Added $U/_priotest to UPROGS.
- user/priotest.c:
  * Forks 4 processes in REVERSE priority order: Child 1 (Prio 20), Child 2 (Prio 12),
    Child 3 (Prio 8), Child 4 (Prio 5).
  * Child 4 (Prio 5, forked LAST) runs FIRST, disproving FCFS.
  * Real-time kernel aging logs print on console as wait_ticks exceeds 30.
  * Measures effective aged priority upon start and uses a pipe to report true
    chronological completion order with aged levels.

3. HOW TO BUILD AND RUN
--------------------------------------------------------------------------------
From this directory:
    $ make qemu

Inside xv6 shell:
    $ priotest

To exit QEMU:
    Press Ctrl-A then x

4. VERIFIED TEST OUTPUT LOG
--------------------------------------------------------------------------------
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
