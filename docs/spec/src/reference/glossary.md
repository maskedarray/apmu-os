# Glossary and sources

## Glossary

APMU
:   Advanced Performance Monitoring Unit of the AlSaqr SoC: event filters,
    32 counters and a small processing element.

PE
:   The APMU's processing element, an Ibex RV32 core with counter instructions.

ISPM / DSPM
:   Instruction (8 KiB) and data (128 KiB) scratchpads of the PE.

apmu-os
:   The runtime on the PE: scheduler, queues, base component.

Base component
:   apmu-os's component 0: dispatches requests, installs and removes
    components. Trusted.

Component
:   Code and state installed on the PE by an application. Today a native RV32
    object; in the target, a set of verified eBPF programs translated to RV32.

Slot
:   (target) A counter granted to a component, numbered 0..n−1 from the
    component's point of view; the kernel maps it to a hardware counter.

Doorbell
:   Counter 0; the host sets its pending bit to wake the base.

Live install
:   Installing code into dynamically allocated, unused ISPM/DSPM while the PE
    runs, then activating it after instruction-fetch synchronization.

Offload device
:   A device that verifies-and-translates BPF programs instead of running them
    on the host CPU (Linux BPF offload; generalised by P2).

Manifest
:   What an install asks for: events, scopes, notifications, budget.

Scope
:   Whose activity a counter may observe: `task`, `core:n`, `system`.

Session
:   An open file of `/dev/apmu`; owns handles.

## Sources

| Topic | Where |
|---|---|
| Hardware reference (registers, events, RTL line numbers, known bugs) | `knowledgebase/apmu.md` |
| Running apmu-os, install history, the hang investigation | `knowledgebase/running-apmu-os.md` |
| Board access and transfer | `knowledgebase/connecting-to-board.md` |
| Project handoff | `knowledgebase/continue-session-apmu-sw.md` |
| Isolation architecture (long form) | `apmu-os/docs/apmu-isolation-architecture.html` |
| apmu-os | `apmu-os/` (`README.md`) |
| Module, library, CLI, tests | `alsaqr-software/linux/apmu/` (`README.md`) |
| Kernel build | `alsaqr-software/linux/CLAUDE.md`; tree on the VM at `~/alsaqr-build/linux/cva6-sdk/buildroot/output/build/linux-v6.1.183` |
| RTL | `he-soc/hardware/ip_list/apmu`, `he-soc/hardware/ip_list/ibex_pmu` |
