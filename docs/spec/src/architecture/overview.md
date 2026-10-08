# Target architecture

<span class="st dec">DECIDED</span> at the level of the decisions listed below;
details marked otherwise are this spec's proposals.

## Decisions this design is built on

| Date | Decision | Where |
|---|---|---|
| 2026-10-06 | The kernel module owns the APMU; userspace passes objects and words. Allocation, linking and ids live on the host. | [Module](../linux/module.md) |
| 2026-10-07 | **The kernel owns the counters.** Components and applications never choose a counter index or write an event selector, event info or budget. | [Security model](security.md) |
| 2026-10-07 | **Components are eBPF, checked by the Linux kernel's own verifier**, using a new program type on a patched kernel, translated to RV32 by apmu.ko. Chosen over PREVAIL in a root daemon. | [Verification](../verification.md), [kernel patches](../kernel/index.md) |
| 2026-10-07 | **Per-task event scope by gating counters on context switches and interrupts** in software. | [Event scopes](scopes.md) |

## Trust zones

```mermaid
flowchart TB
  subgraph U["Userspace · untrusted · any uid"]
    direction LR
    A["Applications<br/>(A, B, … and malicious ones)"]
    L["libapmu + libbpf<br/>no security role"]
  end
  subgraph K["Kernel · trusted"]
    direction LR
    V["BPF verifier<br/>BPF_PROG_TYPE_APMU"]
    KO["apmu.ko · reference monitor<br/>sessions · policy · ledger<br/>translator · broker · supervisor"]
    HK["scope hooks<br/>sched switch, irq"]
  end
  subgraph PE["APMU PE"]
    direction LR
    BASE["apmu-os base<br/>trusted"]
    C["components<br/>verified + translated"]
  end
  HW["APMU hardware: counters, ISPM, DSPM<br/>configured only by the kernel"]
  U -- "bpf(): programs, maps" --> V
  U -- "ioctl / mmap / poll" --> KO
  V -- "offload callbacks" --> KO
  HK --> KO
  KO -- "install records,<br/>tagged requests" --> BASE
  BASE --> C
  KO -- "MMIO" --> HW
  C -- "granted counters only" --> HW
```

Two boundaries matter: **userspace → kernel** (everything is copied and
validated; userspace never names a raw resource) and **kernel → PE**
(the kernel decides what each component may touch, and the translator makes
the component's code unable to touch anything else).

## What a component is

| | Today <span class="st impl">IMPLEMENTED</span> | Target <span class="st dec">DECIDED</span> |
|---|---|---|
| Format | relocatable RV32 ELF object | eBPF object (`clang -target bpf -mcpu=v3`) |
| Entry points | `component_event_handler`, `component_request_handler`, `init_hook`, `exit_hook` | programs in sections `apmu/event`, `apmu/request`, `apmu/init`, `apmu/exit` |
| State | globals in DSPM | BPF array maps (including libbpf's `.bss`/`.data`), bound to the APMU, placed in DSPM |
| Counters | any index; programs its own selector | *slots* 0..n−1 granted by a manifest; the kernel maps slots to hardware counters |
| Checked by | nothing | the kernel verifier + APMU checks + translator |
| Code on the PE | the object's own machine code | RV32 emitted by apmu.ko's translator |
| Host reads its state | by request only | also directly: `bpf_map_lookup_elem()` on its maps (the module reads DSPM) |

## Install, end to end (target)

```mermaid
sequenceDiagram
  autonumber
  participant A as App
  participant L as libbpf
  participant V as Verifier (kernel)
  participant K as apmu.ko
  participant P as apmu-os base
  A->>L: open component.bpf.o
  L->>V: BPF_MAP_CREATE (map_ifindex = APMU device)
  V->>K: map_alloc: reserve DSPM
  L->>V: BPF_PROG_LOAD type APMU, attach type EVENT/REQUEST/…, prog_ifindex = APMU device
  V->>K: prepare(), then insn_hook() per instruction
  V->>V: full verification (unchanged verifier)
  V->>K: finalize(): APMU checks (budget, slots, 32-bit profile)
  V-->>L: prog fd
  A->>K: ioctl INSTALL {prog fds, manifest}
  K->>K: policy (scopes, quotas), ledger (counters, slot, ISPM)
  K->>K: translate programs to RV32, bind slots → counters
  K->>K: program counters, scrub memory
  K->>P: write dynamic ISPM/DSPM ranges, send generation-tagged install
  P-->>K: installed
  K-->>A: opaque handle
```

## Request and reply (target)

```mermaid
sequenceDiagram
  autonumber
  participant A as App (session)
  participant K as apmu.ko broker
  participant P as base
  participant C as component
  A->>K: SEND {handle, words}
  K->>K: admission: in-flight cap, rate, round-robin
  K->>P: request {slot, tag = generation·seq, words}
  P->>C: request program (ctx = copy of words)
  C->>P: bpf_apmu_reply(buf, n)
  P->>K: reply stamped {slot, tag} by the base
  K->>K: route by tag → the sending session, drop stale generations
  K-->>A: RECV / poll()
```

The component never names who it replies to: the base stamps the reply with
the slot and tag of the request it is serving. A reply from an event program
(no request) carries tag 0 and goes to the component's owning session as a
notification.

## Component lifecycle

```mermaid
stateDiagram-v2
  [*] --> Loaded: BPF_PROG_LOAD (verified)
  Loaded --> Installing: INSTALL ioctl
  Installing --> Rejected: policy / translator / no space
  Rejected --> [*]
  Installing --> Running: base confirms
  Running --> Running: requests, events
  Running --> Faulted: trap or budget overrun
  Faulted --> Running: supervisor reinstalls (state reset)
  Faulted --> Evicted: repeated faults
  Running --> Removing: UNINSTALL or fd closed
  Evicted --> Removing
  Removing --> [*]: counters reset, memory scrubbed, generation bumped
```

## What changes in each layer

| Layer | Keeps | Adds | Removes once RTL is fixed |
|---|---|---|---|
| apmu-os | scheduler, queues, base, live component activation, trap record | counter remap, reply stamping, request tags, trace ring | `debug_printf` on the PE |
| apmu.ko | ownership per fd, dynamic allocation/link/install, scrubbing, mailboxes, fault reporting | sessions, policy, counter ledger, translator, offload ops, broker with tags, scope hooks client, base image from firmware | — |
| libapmu | `apmu_call` style API | handle-based API, libbpf helpers | `apmu_boot` for non-root |
| kernel | — | patches P1–P4 | — |
