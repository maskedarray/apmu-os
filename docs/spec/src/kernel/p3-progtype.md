# P3: `BPF_PROG_TYPE_APMU`

<span class="st prop">PROPOSED</span>

## What happens originally (6.1)

- Program types are a fixed list compiled into the kernel
  (`include/linux/bpf_types.h`); each names its verifier ops (context access
  rules, helper prototypes) and prog ops. A module cannot add one.
- Unprivileged users may load only `SOCKET_FILTER` and `CGROUP_SKB`
  programs (`bpf_prog_load()`), and only when
  `kernel.unprivileged_bpf_disabled` is 0 (`__sys_bpf()`), which
  `CONFIG_BPF_UNPRIV_DEFAULT_OFF` sets to 2 at boot.
- The verifier sets its strictness from the caller's capabilities in
  `bpf_check()` (lines 15554–15559):

    ```c
    env->allow_ptr_leaks = bpf_allow_ptr_leaks();
    env->bypass_spec_v1 = bpf_bypass_spec_v1();
    env->bypass_spec_v4 = bpf_bypass_spec_v4();
    env->bpf_capable = bpf_capable();
    ```

    Without `CAP_BPF`, `check_cfg()` rejects any back-edge ("back-edge from
    insn %d to %d", line 11049): **unprivileged programs cannot loop at all**,
    which matches the APMU's v1 profile. Without the bypass flags, the
    verifier adds Spectre v1/v4 sanitation, which only makes sense for code
    running on the host CPU.

## What changes

### UAPI (`include/uapi/linux/bpf.h`, and its copy in `tools/`)

```c
enum bpf_prog_type {
    ...
    BPF_PROG_TYPE_SYSCALL,
    BPF_PROG_TYPE_APMU,         /* new, appended */
};

enum bpf_attach_type {
    ...
    BPF_APMU_INIT,              /* new, appended */
    BPF_APMU_EXIT,
    BPF_APMU_EVENT,
    BPF_APMU_REQUEST,
};

#define __BPF_FUNC_MAPPER(FN)   \
    ...                         \
    FN(user_ringbuf_drain),     \
    FN(apmu_counter_read),      /* new, appended */ \
    FN(apmu_counter_write),     \
    FN(apmu_counter_reset),     \
    FN(apmu_reply),             \
    FN(apmu_cycles),            \
```

New header `include/uapi/linux/apmu_bpf.h` with `struct apmu_ctx`
([components](../apmu-os/components.md#context)).

### Program type (`include/linux/bpf_types.h`, new `kernel/bpf/apmu.c`)

```c
BPF_PROG_TYPE(BPF_PROG_TYPE_APMU, apmu, struct apmu_ctx, struct apmu_ctx)
```

`kernel/bpf/apmu.c` (built-in, ~150 lines) defines:

- **`apmu_is_valid_access()`**: reads only, 4-byte aligned, inside
  `struct apmu_ctx`; `fired` only for `BPF_APMU_EVENT`; `nwords` and `w[]`
  only for `BPF_APMU_REQUEST`; nothing for init/exit.
- **`apmu_func_proto()`**: the five APMU helpers, `bpf_map_lookup_elem`, and
  `bpf_trace_printk` (an APMU-specific proto with the standard signature, so
  `CONFIG_BPF_EVENTS` is not needed). Each proto has a stub `.func` that is
  never executed (the programs are offload-only):

    | Helper | `ret_type` | Arguments |
    |---|---|---|
    | `apmu_counter_read` | `RET_INTEGER` | `ARG_ANYTHING` (slot; constancy checked by the device) |
    | `apmu_counter_write` | `RET_VOID` | `ARG_ANYTHING`, `ARG_ANYTHING` |
    | `apmu_counter_reset` | `RET_VOID` | `ARG_ANYTHING` |
    | `apmu_reply` | `RET_INTEGER` | `ARG_PTR_TO_MEM \| MEM_RDONLY`, `ARG_CONST_SIZE` (bytes, ≤ 256) |
    | `apmu_cycles` | `RET_INTEGER` | — |

- empty `apmu_prog_ops` (no `BPF_PROG_TEST_RUN`).

### Load rules (`kernel/bpf/syscall.c`)

| Place | Change |
|---|---|
| `bpf_prog_load_check_attach()` | for `BPF_PROG_TYPE_APMU`, `expected_attach_type` must be one of the four `BPF_APMU_*` |
| `bpf_prog_load()` | `BPF_PROG_TYPE_APMU` requires `prog_ifindex` (offload-only); it joins `SOCKET_FILTER` and `CGROUP_SKB` in the list unprivileged users may load |
| `__sys_bpf()` | with `unprivileged_bpf_disabled` set, still allow `BPF_PROG_LOAD` of `BPF_PROG_TYPE_APMU`, `BPF_MAP_CREATE` with a generic offload `map_ifindex` whose device allows it, and the map commands on such maps by fd |

The APMU device keeps the final say: its `prepare()` callback and map
`map_alloc()` check the caller against apmu.ko's policy (e.g. membership of
the `apmu` group, the same rule as opening `/dev/apmu`) and fail with
`EPERM` otherwise.

### Verifier (`kernel/bpf/verifier.c`, two lines)

```c
    env->bypass_spec_v1 = bpf_bypass_spec_v1();
    env->bypass_spec_v4 = bpf_bypass_spec_v4();
+   if (env->prog->type == BPF_PROG_TYPE_APMU)   /* never runs on the host CPU */
+       env->bypass_spec_v1 = env->bypass_spec_v4 = true;
    env->bpf_capable = bpf_capable();
```

`allow_ptr_leaks` and `bpf_capable` stay as the caller's, so an unprivileged
APMU program is checked with the same strictness as any unprivileged
program: no loops, no pointer leaks, no reads of uninitialised stack, at most
4,096 instructions.

## What the verifier then guarantees for an APMU program

From the unmodified analysis:

- every memory access is to the context (inside `struct apmu_ctx`, read-only),
  the 512-byte stack, or a map value, within bounds;
- no uninitialised reads; no pointer arithmetic that escapes an object;
- only the helpers above, with correctly typed arguments;
- no back-edges (unprivileged), so every run is finite;
- no unreachable code, valid jumps, a single `exit` per path.

What it does not check, and the APMU device does in `insn_hook`/`finalize`:
slot arguments constant and in range, the 32-bit profile, the cycle budget.
See [Verification](../verification.md).
