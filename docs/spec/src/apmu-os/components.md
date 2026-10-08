# Components

A component is the unit an application installs on the PE: event-driven code
plus state, reached by requests.

## Today: native RV32 components <span class="st impl">IMPLEMENTED</span>

Written in C against `apmu_component.h`, built by `make components` into a
relocatable object. The kernel links it (`apmu_link.c`: places sections,
applies the RISC-V relocations, resolves calls to the base's exports).

```c
--8<-- "apmu-os/common/include/apmu_component.h:4:"
```

Example, `hello` (replies with each word + 1):

```c
--8<-- "apmu-os/components/hello/hello.c"
```

Example, `latency_binning` (histogram of DRAM read latency, counter 1):

```c
--8<-- "apmu-os/components/latency_binning/latency_binning.c"
```

Note what the target design takes away from it: it picks counter 1 itself and
writes `EventSel`/`EventInfo` in `init_hook` and in request 3.

## Target: eBPF components <span class="st dec">DECIDED</span>

### Programs and sections

A component is one eBPF object with up to four programs, all of type
`BPF_PROG_TYPE_APMU`, distinguished by `expected_attach_type`:

| Section | Attach type | When it runs | ctx fields readable |
|---|---|---|---|
| `apmu/init` | `BPF_APMU_INIT` | once, at install, before events are enabled | none |
| `apmu/exit` | `BPF_APMU_EXIT` | once, at uninstall | none |
| `apmu/event` | `BPF_APMU_EVENT` | when any of its slots is pending | `fired` (slot bits) |
| `apmu/request` | `BPF_APMU_REQUEST` | for each request to the component | `nwords`, `w[0..63]` |

### Context

```c
/* include/uapi/linux/apmu_bpf.h (kernel patch P3) */
struct apmu_ctx {
    __u32 fired;            /* EVENT: bit k = slot k fired */
    __u32 nwords;           /* REQUEST: number of valid words in w */
    __u32 w[64];            /* REQUEST: the request, read-only */
};
```

The verifier enforces per attach type which fields may be read (all are
read-only). `w[i]` with a variable `i` must be bounds-checked against 64
by the program, as with any BPF context.

### Helpers

| Helper | Proto | Translates to |
|---|---|---|
| `bpf_apmu_counter_read(slot)` | `u32 (u32 slot)` | `cnt.rd` with the bound hardware index |
| `bpf_apmu_counter_write(slot, v)` | `void (u32 slot, u32 v)` | `cnt.wr`, value masked to 30 bits |
| `bpf_apmu_counter_reset(slot)` | `void (u32 slot)` | store 0 to the counter register (clears value and pending) |
| `bpf_apmu_reply(buf, nwords)` | `long (const void *buf, u32 n)`, `buf` stack or map value, n ≤ 64 | call to the base's `apmu_reply` |
| `bpf_apmu_cycles()` | `u32 (void)` | `csrr mcycle` |
| `bpf_trace_printk(fmt, size, a, b, c)` | standard | call to the base's `apmu_trace` with the format's offset in `.rodata` |
| `bpf_map_lookup_elem(map, key)` | standard, **array maps only** | inline bounds check and address computation |

`slot` must be a constant smaller than the number of granted slots; the APMU
check rejects anything else (see [Verification](../verification.md)).

### State

Global variables become libbpf's `.bss`/`.data`/`.rodata` array maps. All of a
component's maps must be created bound to the APMU (`map_ifindex`), and the
module places them in the component's DSPM region. Applications can read
them directly with `bpf_map_lookup_elem()` on the map fd: the module's map
operations read DSPM.

Only `BPF_MAP_TYPE_ARRAY` is supported at first.

### Example: `latency_binning` as eBPF

```c
// latency_binning.bpf.c: histogram of DRAM read latency.
// The counter is granted by the manifest (slot 0: DRAM read response,
// op max(latency)); the component no longer programs it.
#include <linux/bpf.h>
#include <linux/apmu_bpf.h>
#include <bpf/bpf_helpers.h>

#define NBINS 10

__u32 bin_lo[NBINS] = { 0, 4, 8, 12, 16, 20, 24, 32, 48, 64 };
__u32 bin_value[NBINS];

SEC("apmu/event")
int on_event(struct apmu_ctx *ctx)
{
    __u32 lat = bpf_apmu_counter_read(0);

    #pragma unroll
    for (int i = NBINS - 1; i >= 0; i--)
        if (lat >= bin_lo[i]) {
            bin_value[i]++;
            break;
        }
    bpf_apmu_counter_reset(0);
    return 0;
}

SEC("apmu/request")
int on_request(struct apmu_ctx *ctx)
{
    if (ctx->nwords < 1)
        return 0;
    switch (ctx->w[0]) {
    case 1:                                     /* clear */
        __builtin_memset(bin_value, 0, sizeof(bin_value));
        break;
    case 2:                                     /* read */
        bpf_apmu_reply(bin_value, NBINS);
        break;
    case 4:                                     /* set bounds */
        if (ctx->nwords < 1 + NBINS)
            return 0;
        #pragma unroll
        for (int i = 0; i < NBINS; i++)
            bin_lo[i] = ctx->w[1 + i];
        __builtin_memset(bin_value, 0, sizeof(bin_value));
        break;
    }
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
```

Request 3 (choose the core) disappears from the component: changing which
core is observed is a change of the counter's selector, so it is a manifest
choice (and needs the scope for that core).

### Manifest that goes with it

```c
struct apmu_manifest m = {
    .nslots = 1,
    .slot[0] = { .cls = APMU_EV_DRAM, .event = APMU_EV_RD_RES,
                 .op = APMU_OP_MAX, .info_lo = 0, .info_hi = 15,
                 .scope = APMU_SCOPE_SYSTEM },
    .budget_us = 20,
};
```

### Programming rules (v1 profile)

These are what the APMU checks enforce on top of the kernel verifier
(details on [Verification](../verification.md)):

- 32-bit arithmetic on scalars (`-mcpu=v3`, `__u32` types); 64-bit ALU only on
  pointers.
- No loops the compiler cannot unroll; no BPF-to-BPF calls (`__always_inline`).
- Counters only through the helpers, slots as constants.
- The longest path must fit the requested `budget_us`.
