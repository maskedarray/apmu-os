# Security model

The long-form argument is in [the isolation architecture](../isolation-architecture.html).
This page is the normative summary that the implementation must satisfy.

## Threat model

- **Trusted:** the Linux kernel including apmu.ko and the patched verifier, the
  apmu-os base image (loaded by the kernel), the hardware, and the
  administrator who sets policy.
- **Untrusted:** every userspace process, whatever its uid, including colluding
  processes and processes that fork, pass file descriptors, crash mid-call, or
  issue arbitrary ioctls and `bpf()` calls with crafted arguments and programs.
- **Out of scope:** what a component computes from the resources it was
  granted (separate work).

## Goals

| | Goal | Means |
|---|---|---|
| G1 | **Confidentiality.** An application learns only events and data it is entitled to. | Event scopes ([scopes](scopes.md)); components reach only their own memory and counters ([verification](../verification.md)); replies routed by the base's stamp |
| G2 | **Integrity.** No application can alter another's components, counters, messages or results. | Ownership per session; kernel-only counter configuration; translator masks; generation-tagged replies |
| G3 | **Fair availability.** No application can exhaust slots, memory, counters, queue space or PE time. | Quotas per user and session; in-flight caps; budgets checked at install; eviction |
| G4 | **System safety.** No unprivileged application can change SoC configuration, interrupt routing or core behaviour. | Regulation, EVU export, MemGuard period and IRQ routing are administrator-only |

## Rules

1. **Nobody but the kernel names a raw resource.** Userspace never passes a
   counter number, register address, physical address or component slot that
   the kernel acts on directly. Handles are opaque.
2. **The kernel is the only writer of APMU configuration** (selectors, event
   info, budgets, period, IRQ enables, EVU export).
3. **Every object belongs to one session** (an open file), every operation is
   checked against that, and sharing happens only through an explicit grant.
4. **Every hand-over is scrubbed:** counters reset and selectors cleared, ISPM
   and DSPM ranges zeroed, mailboxes and in-flight replies dropped, generation
   bumped.
5. **Everything is bounded per user:** slots, bytes, counters, in-flight
   requests, message rate, PE time.

## Who controls what

| Resource | Configured by | Untrusted app may request | Kernel checks and does | Component code may |
|---|---|---|---|---|
| Counter + event selector | kernel | an event in abstract terms (class, event, op, scope) in the manifest | scope allowed ([scopes](scopes.md)); quota; translates to port/source/event masks; resets; records owner | `bpf_apmu_counter_read/write/reset` on its own slots |
| Event info / ALU op | kernel | part of the event request | op from a fixed catalogue (no `KEEP_MIN`, `ADD_CMP_EQ/NEQ`) | nothing |
| Budgets, period, overflow IRQ | kernel | "notify me on overflow of slot k" | notification for own counters only; budgets and regulation are admin-only | nothing |
| CVA6 EVU export (`0x10606000`) | kernel | nothing directly | admin policy | nothing |
| ISPM / DSPM | kernel | implicitly, by program and map size | per-user byte quota; zeroed before reuse | its own region only |
| Component slot (1–9) | kernel | install | slot quota; slots never shown to users | nothing |
| Request queue | kernel writes | send ≤ 64 words to a handle it holds | in-flight cap, rate, round-robin, tag | read its own request (ctx) |
| Response queue | base writes, kernel reads | receive/poll on its handle | routes by the base's stamp only | `bpf_apmu_reply` |
| apmu-os image, stall, reset | kernel | nothing | base from `/lib/firmware`; reset by supervisor or admin | nothing |

## The manifest

What an install asks for. The kernel grants exactly that or refuses.

```c
struct apmu_manifest {          /* see the UAPI page for the exact layout */
    __u32 nslots;                       /* counters requested, ≤ quota */
    struct apmu_event_req slot[8];      /* class, event, op, info slice, scope */
    __u32 notify_overflow;              /* slots whose overflow reaches the session */
    __u32 budget_us;                    /* longest handler run requested */
};
```

The kernel then binds (none of this is visible to the application):

- slot *k* → hardware counter *i*, used by the translator when it emits `cnt.*`;
- the wake-up mask (hardware bits of the granted slots), installed in the base;
- the component's DSPM region, used by the translator's address masks.

## Remaining risks

- **Timing.** Components share one PE; reply latency reveals other components'
  activity. Caps and budgets limit but do not remove it.
- **Kernel-mode activity on a scoped core** is visible to per-task scopes
  unless interrupts are gated too ([scopes](scopes.md)).
- **Verifier bugs.** Mitigated by the translator's runtime masks, so a
  mis-verified program still cannot leave its region.
