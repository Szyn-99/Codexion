*This project has been created as part of the 42 curriculum by aymel-ha.*

# Codexion — Race Against the Deadline

> *"The challenge is not writing the code — it is writing it before the deadline writes you off."*

---

## Description

Ever wondered what happens when 200 developers fight over 4 USB dongles? Welcome to **Codexion**.

This project is a turbocharged spin on the classic [Dining Philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), rebuilt for the modern coding hub. Instead of philosophers and forks, you have coders and USB dongles — and instead of spaghetti, there's quantum code to compile before someone burns out.

Each coder sits around a shared Quantum Compiler, cycling through three phases:
- **Compiling** — requires two dongles simultaneously (one in each hand)
- **Debugging** — solo work, no dongles needed
- **Refactoring** — solo work, then back to compiling they go

The simulation ends when either:
- Every coder has compiled enough times.
- Some Coder burns out from dongle starvation.

Under the hood, this is a deep dive into **POSIX threads, mutex synchronization, condition variables, priority queues, and scheduling policies (FIFO & EDF)** — all written in C, from scratch, with zero global variables.

---

## Instructions

### Building

Navigate into the source directory and run:

```bash
cd coders
make
```

This compiles with `-Wall -Wextra -Werror -pthread`. Clean build every time.

To clean up:
```bash
make fclean   # removes binaries and objects
make re       # full rebuild from scratch
```

### Running

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> \
           <time_to_debug> <time_to_refactor> \
           <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

| Argument | What it means |
|---|---|
| `number_of_coders` | How many coders (= how many dongles) |
| `time_to_burnout` | Max time (ms) a coder can go without starting a compile |
| `time_to_compile` | How long compiling takes (ms) |
| `time_to_debug` | How long debugging takes (ms) |
| `time_to_refactor` | How long refactoring takes (ms) |
| `number_of_compiles_required` | Compiles needed per coder to win |
| `dongle_cooldown` | How long a dongle is locked after release (ms) |
| `scheduler` | `fifo` (first come first served) or `edf` (earliest deadline first) |

### Example

```bash
# 5 coders, 800ms burnout limit, 200ms compile, EDF scheduling
./codexion 5 800 200 50 30 10 100 edf
```

## Resources

- **[The Linux Programming Interface — Michael Kerrisk](https://man7.org/tlpi/)**
  The bible of Linux system programming. Chapters 29–33 on POSIX threads were instrumental in understanding how `pthread_create`, `pthread_mutex_t`, and `pthread_cond_t` actually work under the hood — not just the API, but the *why*. Particularly eye-opening: how condition variables interact with mutexes to guard against spurious wakeups and missed signals, which directly shaped the design of `wait_for_usb` and `bring_two_usbs`.

- **[Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)**
  The conceptual ancestor of this project. Understanding Dijkstra's original formulation (and why naive solutions deadlock) frames everything here.

- **[Deadlock — Coffman Conditions](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)**
  The four conditions (mutual exclusion, hold and wait, no preemption, circular wait) and why breaking even one of them is enough to prevent deadlock entirely.

- **AI Integration**
  AI assistance was used throughout the project for: analyzing the resource ordering strategy to prevent circular-wait deadlocks, auditing memory safety and identifying a Use-After-Free risk in the monitor shutdown path, simplifying the `wait_for_usb` timespec calculation, and reviewing EDF priority queue correctness. All generated logic was reviewed, understood, and validated before integration.

---

## Blocking Cases Handled

### Deadlock Prevention
The classic Dining Philosophers trap: two adjacent coders each grab one dongle and wait forever for the other. We prevent this with **resource ordering** inside `hand_dongles()` — every coder always locks the dongle with the lower array index first. This breaks the circular-wait Coffman condition entirely. No deadlock is possible.

### Starvation Prevention
With EDF scheduling, dongles are granted to the coder whose burnout deadline is closest. This means nobody gets stuck behind a coder who has plenty of time left — the most at-risk coder always gets priority. The scheduler is backed by a custom 2-slot priority heap (`priority_queue.c`) that handles the FIFO/EDF arbitration cleanly.

### Cooldown Handling
After a coder releases a dongle, it becomes unavailable for `dongle_cooldown` milliseconds. Rather than spinning in a busy loop, waiting coders are suspended via `pthread_cond_timedwait` with a deadline set directly from the dongle's `served_at` field. No wasted CPU cycles.

### Precise Burnout Detection
A dedicated monitor thread checks every half-millisecond (`usleep(500)`) whether any coder has exceeded their burnout threshold. When one is detected, the burnout log is printed and the simulation is cleanly stopped — guaranteed within 10ms of the actual burnout moment, as required by the subject.

### Log Serialization
All output goes through a single `log_mutex`. This ensures that two coder messages never interleave on the same line, even when 200 threads are trying to write simultaneously.

---

## Thread Synchronization Mechanisms

### `pthread_mutex_t`
Used in three places:
- **Per-dongle mutex** (`usb_mutex`): Guards each dongle's state (free/taken, queue, cooldown timer). Coders lock this before reading or modifying a dongle.
- **`codex_mutex`**: Protects the global simulation status flag (`codex_over`). Used by the monitor to signal termination, and by coders to check if they should stop.
- **`log_mutex`**: Serializes all `printf` calls so output never gets mangled.

### `pthread_cond_t`
Each dongle has a `usb_cond` condition variable. When a coder needs a dongle that isn't ready (either busy or in cooldown), it waits on the condition variable instead of spinning. When a dongle is released, `pthread_cond_broadcast` wakes all waiters, and the scheduler's priority queue decides who gets it next.

### Race Condition Prevention
- The `codex_over` flag is **always** read and written under `codex_mutex` — never bare.
- Dongle state is **always** accessed under `usb_mutex` — condition checks, queue updates, and state transitions are atomic with respect to other threads.
- The two-dongle acquisition in `bring_two_usbs` carefully releases and re-acquires mutexes in a defined order, avoiding the classic ABBA deadlock pattern.

### Safe Monitor to Coder Communication
The monitor thread sets `codex_over = 1` under `codex_mutex`, then broadcasts on all condition variables to wake sleeping coders. Coders check the flag regularly inside their main loop and exit gracefully without touching freed memory.
