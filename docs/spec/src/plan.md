# Implementation plan

<span class="st prop">PROPOSED</span> order. Each milestone ends with something
that runs on the board, and updates this site.

## Milestones

Each milestone builds on the previous one; M1 can start before the RTL fixes are in.


| # | Milestone | Deliverables | Done when |
|---|---|---|---|
| M1 | **Translator offline** | translator as a userspace library sharing code with the module (like `apmu_link.c` today); eBPF versions of `hello` and `latency_binning`; differential test harness (interpreter vs RV32 in QEMU) | both components translate and match the interpreter on random inputs; **code size measured** and the ISPM decision taken |
| M2 | **Verifier on the board** | patches P1–P3 as a series, kernel builds and boots, libbpf on the image | an unprivileged user loads an APMU program bound to a stub offload device and gets the verifier log for a bad one |
| M3 | **End to end** | apmu.ko registers the offload device (`prepare`/`insn_hook`/`finalize`/`translate`, map ops); base implements ABI v2 install records, reply stamping, trace ring | `latency_binning.bpf.o` installed via `INSTALL2`, histogram read by request and by map lookup |
| M4 | **Kernel owns counters** | event catalogue, manifest, slot binding, selector programming, `CAP_PERFMON` gate for system scope | a component cannot observe an event it was not granted; old native install is root-only |
| M5 | **Multi-tenant** | sessions, opaque handles with generations, quotas, in-flight caps, round-robin, scrubbing, `poll()`, base from `request_firmware`, `debug=1` gate | two users' components run side by side; stress tests from today pass on v2 |
| M6 | **Task scope** | P4 hooks, scope client in apmu.ko | a task-scoped counter counts only its task (measured with a traffic generator and an interfering task) |

## Tests to carry forward

| Today's test | v2 equivalent |
|---|---|
| `make test-link` (kernel linker vs `ld.lld`) | translator differential test |
| `make test-queue` (host vs PE queue) | same, for ABI v2 objects |
| board-script boot, bubble, hello and repeated-request sanity | same over `INSTALL2`/`SEND2` |
| — | negative suite: programs that must be rejected (each constraint in [Verification](verification.md)); programs that try to escape and must stay inside |
| — | multi-session isolation: session B cannot see, use or infer handles of A; stale handles fail |

## Risks

| Risk | Impact | Mitigation | Status |
|---|---|---|---|
| **ISPM capacity** for translated code (5,560 B free today; ~5.8–6.2 KiB after further slimming; one translated `latency_binning` estimated at 0.8–1.5 KiB) | a finite number of components at a time | measure in M1; compressed instructions; elide masks; 8 KiB ISPM is implemented | <span class="st impl">MITIGATED</span> |
| 32-bit profile too restrictive | real components need 64-bit arithmetic | full 64-bit emulation as in `bpf_jit_comp32.c` | <span class="st open">OPEN</span> |
| Unprivileged programs cannot loop | some components need loops | unrolling; later: privileged loops with fuel, or bounded loops for `CAP_BPF` users | accepted for v1 |
| P2 depends on 6.1 offload internals | rebase cost | keep P2 small; it is the one patch to redo on a newer kernel | accepted |
| No PE timer | an overrunning component stalls everyone until recovery | static budgets; hardware change requested | accepted |
| Module unload corrupts this kernel | development friction | test new modules on a fresh boot, or build apmu.ko into the kernel | known |

## Open questions

1. With the implemented 8 KiB ISPM, how many realistic translated components fit?
2. Does the PE's Ibex have the C extension enabled (RTL says RV32IMC)?
3. Who may install unprivileged: everyone, or the `apmu` group?
4. Is `task` scope allowed without `CAP_PERFMON` given it includes the task's
   own kernel-mode activity (Linux `perf_event_paranoid = 1` semantics)?
5. Should the base keep a minimal `printf` for debugging the base itself, or
   move fully to the trace ring?
