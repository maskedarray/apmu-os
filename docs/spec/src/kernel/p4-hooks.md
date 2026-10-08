# P4: scope hooks

<span class="st prop">PROPOSED</span>. The design is on [Event scopes](../architecture/scopes.md).

## What happens originally (6.1)

- **Context switch.** `prepare_task_switch()` in `kernel/sched/core.c`
  (line 5050) already calls the switch-out notifiers of perf and of preempt
  notifiers:

    ```c
    static inline void
    prepare_task_switch(struct rq *rq, struct task_struct *prev,
                        struct task_struct *next)
    {
        kcov_prepare_switch(prev);
        sched_info_switch(rq, prev, next);
        perf_event_task_sched_out(prev, next);
        rseq_preempt(prev);
        fire_sched_out_preempt_notifiers(prev, next);
        kmap_local_sched_out();
        prepare_task(next);
        prepare_arch_switch(next);
    }
    ```

    Preempt notifiers would do for one task, but they are registered per
    task, only by that task itself, and `CONFIG_PREEMPT_NOTIFIERS` has no
    prompt (only KVM selects it). They cannot follow all threads of a process
    or be armed for a task that never calls into apmu.ko.

- **Interrupts.** On RISC-V 6.1, `entry.S` dispatches every interrupt to
  `generic_handle_arch_irq()` (`kernel/irq/handle.c`):

    ```c
    asmlinkage void noinstr generic_handle_arch_irq(struct pt_regs *regs)
    {
        irq_enter();
        old_regs = set_irq_regs(regs);
        handle_arch_irq(regs);
        set_irq_regs(old_regs);
        irq_exit();          /* runs pending softirqs */
    }
    ```

    `irq_enter_rcu()` and `irq_exit_rcu()` (`kernel/softirq.c`, lines 636
    and 689) are the generic points, and softirqs run inside `irq_exit_rcu()`.

## What changes

A tiny hook interface, compiled in with `CONFIG_APMU_HOOKS`, behind a static
key so it costs a no-op branch when no task-scoped counter exists.

```c
/* include/linux/apmu_hooks.h */
struct apmu_hooks {
    void (*sched_in)(struct task_struct *next, int cpu);
    void (*sched_out)(struct task_struct *prev, int cpu);
    void (*irq_enter)(int cpu);     /* outermost entry only */
    void (*irq_exit)(int cpu);      /* outermost exit, after softirqs */
};
int  apmu_hooks_register(const struct apmu_hooks *h);  /* one user: apmu.ko */
void apmu_hooks_unregister(void);
void apmu_hooks_enable(bool on);                        /* flips the static key */
```

Call sites:

| File | Where | Call |
|---|---|---|
| `kernel/sched/core.c` | `prepare_task_switch()`, after `perf_event_task_sched_out()` | `apmu_hook_sched_out(prev)` |
| `kernel/sched/core.c` | `finish_task_switch()`, next to `perf_event_task_sched_in()` | `apmu_hook_sched_in(current)` |
| `kernel/softirq.c` | start of `irq_enter_rcu()` | `apmu_hook_irq_enter()` |
| `kernel/softirq.c` | end of `irq_exit_rcu()` (after `__irq_exit_rcu()`) | `apmu_hook_irq_exit()` |

`kernel/apmu_hooks.c` keeps a per-CPU interrupt nesting depth and calls the
registered callbacks only on the outermost transition. Callbacks run with
interrupts off and must not sleep; apmu.ko's callbacks only write
`EventSel` registers from a per-CPU list.

Which task is "scoped" is apmu.ko's business: it marks the `task_struct`s of
sessions with `task`-scoped counters (a field `apmu_scoped` added to
`task_struct` under `CONFIG_APMU_HOOKS`, inherited on `fork()` for threads of
the same process) so the hot path is one flag test.

## Cost

| Situation | Cost per context switch or interrupt |
|---|---|
| no task-scoped counters (static key off) | one patched-out branch |
| on, task not scoped | flag test |
| on, task scoped | one 32-bit MMIO write per counter (1 typical) |

## Alternatives considered

| Alternative | Why not |
|---|---|
| preempt notifiers | per-task registration by the task itself; cannot cover all threads |
| a `sched_switch` tracepoint probe from the module | needs tracing configured, still misses interrupts |
| a perf PMU driver for the APMU | perf already switches per-task events, but has no interrupt gating and does not fit the component model; may still be worth adding later for plain counting |
