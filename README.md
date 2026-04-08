*This project has been created as part of the 42 curriculum by aymel-ha.*

# Codexion: Master the race for resources before the deadline masters you

## Description
Codexion is an advanced thread-orchestration and synchronization project challenging developers to coordinate multiple coders competing for limited USB dongles using POSIX threads, mutexes, and smart scheduling policies (FIFO and EDF). Coders cycle through compiling, debugging, and refactoring phases while adhering to strict cooldown barriers and burnout timelines.

## Instructions
**Compilation:**
To compile the system, leverage the included Makefile. It strictly processes via `cc` with all standard 42 verification flags (`-Wall -Wextra -Werror` and tightly integrates `-pthread` for POSIX multithreading):
```bash
cd coders
make
```

**Execution:**
Execute the compiled `codexion` binary with precisely 8 arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```
*Example:* Minimum 200 coders relying on EDF scheduler to prevent burnout
```bash
./codexion 200 620 200 5 3 50 100 edf
```

## Resources
- [POSIX Threads Programming (Pthreads)](https://computing.llnl.gov/tutorials/pthreads/)
- [Coffman's Deadlock Conditions](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)
- [Dining Philosophers Problem Analysis](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- **AI Integration**: AI agents provided critical insights mathematically structuring the resource indexing hierarchy to securely prevent Dijkstra deadlocks, isolating exact POSIX condition variables to properly enforce waiting periods, conducting extensive memory-safety audits resulting in the deployment of `clean_abort` teardowns to eradicate Use-After-Frees, and validating Priority Queue math parameters mapping custom `time_to_burnout` EDF thresholds securely.

## Blocking cases handled
- **Deadlock Prevention (Coffman's Conditions):** A specific memory resource hierarchy was rigorously mapped out dynamically in `hand_dongles()` to guarantee total thread parameter parity. Coders inherently lock their `lowest_usb` structure pointer before their `highest_usb` address constraint, permanently shutting down parallel sequence circular locks.
- **Starvation Prevention:** A robust 2-element Priority Heap (`priority_queue.c`) tracks and governs dongles dynamically against "Earliest Deadline" criteria mapped onto each waiting coder reducing unfair thread scheduling metrics. 
- **Cooldown Handling:** Coder release mechanisms append a specific absolute duration threshold onto `served_at`, pushing subsequent callers inside a `pthread_cond_timedwait` execution natively avoiding active loops.
- **Precise Burnout Detection:** A totally independent master Monitor thread polls all nodes iterating aggressively within a `usleep(1000)` timeline to safely assert that elapsed timelines meet `t_burnout` parameters mathematically isolating and terminating logs safely `< 10ms`.
- **Log Serialization:** Console standard output limits overlap or interleave messages distinctly via the `log_mutex` constraint guaranteeing sequential data parsing.

## Thread synchronization mechanisms
- **`pthread_mutex_t`**: Mutex logic rigorously wraps explicit parameter boundaries guaranteeing mutual exclusion against the `codex_over` check protocols utilizing the structural `codex_mutex`. Additionally, each dongle independently locks via localized bounds mitigating thread collision safely.
- **`pthread_cond_t`**: Event signal handling operates seamlessly utilizing Condition Variables across queue cycles ensuring optimal CPU efficiency. Released dongles intrinsically transmit `pthread_cond_broadcast` signaling suspended systems into targeted wakeups resolving exact access protocols.
- **Race Conditions Prevented**: Data races are thoroughly eradicated globally by protecting evaluation iterations (like comparing integer compilation cycles spanning `n_coders`) securely within distinct master read/write locks (`is_done()` functionality limits collision natively). 
- **Safe Shared State Communication**: Monitor orchestration communicates with standard parallel threads uniquely via boolean flag modification (`modify_sim_status`) strictly governed by `codex_mutex`, preventing corrupted teardowns prior to runtime thread execution shutdowns entirely.
