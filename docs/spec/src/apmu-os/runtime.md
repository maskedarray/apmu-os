# apmu-os runtime

The bare-metal runtime on the PE. Allocation, object linking, IDs, and policy
stay in apmu.ko; the PE runtime initializes queues, dispatches events and
requests, and maintains the component table.

## Today <span class="st impl">IMPLEMENTED</span>

### Source layout

| File | Role |
|---|---|
| `crt0.s` | reset/trap entry and `.bss` clear |
| `main.c` | cold `main()` |
| `common/events.c` | event scheduler and handler registry |
| `common/queue_lib.c` | SPSC request/response queues in DSPM |
| `common/base_component.c` | id-0 dispatch and slot publish/unpublish |
| `common/debug_printf.c` | print buffer at DSPM `+0x1800` |
| `common/timer.c` | `mcycle` helpers |
| `linker.ld` | ISPM `0x10427000`, DSPM `0x10429000`, 0x300 B stack |

Built with clang (`-O2 -flto`, `rv32im_zicsr`), the clean base uses
**2,632 B** of the 8,192 B ISPM, leaving **5,560 B** for components. Its
initialized DSPM image is 160 B.

### Memory map

```text
ISPM 0x10427000 ┌──────────────────────────────┐
                │ base image (2,632 B)         │
     +0x0A48    ├──────────────────────────────┤  _dyn_ispm_start
                │ dynamic components (5,560 B) │
     +0x2000    └──────────────────────────────┘

DSPM 0x10429000 ┌──────────────────────────────┐
                │ .rodata .data .bss .stack    │  0x0000–0x1000
     +0x1000    │ ABI header                   │
     +0x1100    │ request queue (0x300)        │
     +0x1400    │ response queue (0x300)       │
     +0x1800    │ print buffer                 │
     +0x2000    │ dynamic component data       │  to 0x10000
                └──────────────────────────────┘
```

### Boot and live publish

Cold boot clears `.bss`, initializes the print area, publishes the export
table and dynamic ISPM start, initializes both queues, installs the base
component, and writes the ready word.

The module writes a linked component into dynamically allocated ISPM/DSPM while
the PE keeps running, then queues a generation-tagged `INSTALL` message. The
base executes `fence.i`, registers the handlers, and acknowledges.
`UNINSTALL` disables the handler and runs its exit hook before acknowledging;
the kernel then scrubs and releases the ranges.

Clearing a host-requested PE stall remains a restart, but it is always a cold
boot. There is no parking or warm-resume path.

Traps enter the image start because Ibex keeps only `mtvec[31:8]`.
`crt0.s` records `mcause`, `mepc`, and `mtval` at DSPM `+0x1018`,
then performs a cold boot.

### Scheduler and base component

The base component is id 0 and listens on doorbell counter 0. The scheduler
waits on the OR of registered masks and calls matching handlers in ID order.

The base drains request objects `{size, component_id, payload}`. Requests for
id 0 implement `INSTALL` and `UNINSTALL`; other requests call the installed
component's request handler. Base replies are
`{op, status, id}`.

The fixed ispm8k RTL permits ordinary volatile DSPM loads and stores. The old
`safe_ld`/`safe_st` retries and reply checksums have been removed.

## Target <span class="st dec">DECIDED</span> / <span class="st prop">PROPOSED</span>

The base stays the only trusted code on the PE. Its job narrows to
dispatching and stamping; it gains no hardware-configuration API.

### Changes

| Change | Why | Status |
|---|---|---|
| **Component record v2**: `{slot, gen, wake_mask, entry[4], region_base, region_size}` | wake mask and region come from the kernel, not the object | <span class="st dec">DECIDED</span> |
| **Request tags**: each request carries `tag = gen << 16 | seq`; the base keeps the tag of the request it is serving | replies routed by the base, not the component | <span class="st dec">DECIDED</span> |
| **Reply stamping**: the exported reply function takes only `(buf, n)`; the base adds `slot` and `tag` | a component cannot reply as someone else | <span class="st dec">DECIDED</span> |
| **Event fired mask in slot bits**: before calling an event program, the base converts hardware bits to the component's slot bits (`fired_slots`) | components never see hardware counter numbers | <span class="st prop">PROPOSED</span> |
| **Context buffer per component** in its own DSPM region: the base copies the request words (or fired mask) there and passes its address | the program's ctx is inside its masked region | <span class="st prop">PROPOSED</span> |
| **Per-component BPF stack** (512 B) at the start of the component's DSPM region; translated code saves registers on the base's own stack | the region mask covers the stack; saved return addresses are out of the component's reach | <span class="st prop">PROPOSED</span> |
| **Trace ring instead of `printf`**: components log `{slot, fmt offset, 3 args}`; the host formats with the program's own `.rodata` | removes 612 B of `debug_printf` from ISPM | <span class="st prop">PROPOSED</span> |
| Remove `safe_ld`/`safe_st` and reply sums | not needed once the RTL is fixed | <span class="st impl">IMPLEMENTED</span> |
| Handler run-time accounting with `mcycle` around each call; overruns reported to the host | supervisor evidence for eviction | <span class="st prop">PROPOSED</span> |

### Calling convention for translated programs

```text
a0      = ctx pointer (component's ctx buffer in DSPM)
ra      = return into the base dispatcher
sp      = the base's stack (the program's BPF stack is in its own region)
return  = a0 (u32), ignored except by init (non-zero: install fails)
saved   = s0–s11 preserved by the program (translator prologue/epilogue)
```

Exported base functions callable from translated code:

| Export | Signature | Use |
|---|---|---|
| `apmu_reply` | `int (const u32 *buf, u32 n)` | `bpf_apmu_reply` |
| `apmu_trace` | `void (u32 fmt_off, u32 a, u32 b, u32 c)` | `bpf_trace_printk` replacement |

Counter access is not a call: the translator emits `cnt.rd` / `cnt.wr` with
the bound hardware index directly.

### ISPM budget

| Part | Today | Target estimate |
|---|---|---|
| base | 2,632 B | ≈ 2,000–2,400 B (no `printf`, simpler install) |
| left for components | 5,560 B | ≈ 5,792–6,192 B |

The 8 KiB ISPM is now implemented and hardware-tested. Translated eBPF remains
larger than hand-written RV32 (see
[translator](../linux/translator.md#code-size)), so the first implementation
milestone still measures how many realistic components fit.
