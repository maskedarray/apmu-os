# Userspace API

## Today <span class="st impl">IMPLEMENTED</span>

### ioctls on `/dev/apmu`

| ioctl | Argument | Who | Effect |
|---|---|---|---|
| `APMU_IOC_BOOT` | `struct apmu_boot` | `CAP_SYS_ADMIN` | load and cold-start apmu-os; removes all components |
| `APMU_IOC_INSTALL` | `struct apmu_install` | anyone (`PERSIST`: admin) | link and install an RV32 object; returns the id |
| `APMU_IOC_UNINSTALL` | `__u32 id` | owner (persistent: admin) | remove |
| `APMU_IOC_SEND` | `struct apmu_msg` | owner, or anyone for persistent | queue a request (≤ 64 words) |
| `APMU_IOC_RECV` | `struct apmu_msg` | same | next response, waiting up to `timeout_ms` |
| `APMU_IOC_INFO` | `struct apmu_info` | anyone | apmu-os state, every component's placement |

The structures are in [`apmu_ioctl.h`](../reference/headers.md#apmu_ioctlh).

Errors: `ENOENT` unknown or not-owned id, `ENODEV` apmu-os not running,
`EIO` apmu-os failed (recovery attempted), `ETIMEDOUT` no response,
`EAGAIN` request queue full, `ENOSPC` no ISPM/DSPM, `EBUSY` no free id,
`ENOEXEC` the object did not link (`err` says why), `EPERM` privileged
operation.

### libapmu

```c
--8<-- "alsaqr-software/linux/apmu/libapmu.h:23:52"
```

### CLI

```text
apmu boot TEXT.bin DATA.bin   load and start apmu-os (root)
apmu install OBJ              install until uninstalled; prints its id
apmu uninstall ID
apmu list                     state and installed components
apmu req ID [WORD...]         send a request, print the response
apmu recv ID                  print the next response
apmu run OBJ [WORD...]        install, one request, remove
```

## Target: UAPI v2 <span class="st prop">PROPOSED</span>

Programs and maps are created with the standard `bpf()` syscall (through
libbpf), bound to the APMU with its **offload device id**. `/dev/apmu` then
installs a set of verified programs under a manifest and carries messages.

### Device id

```c
#define APMU_IOC_DEVID  _IOR('A', 16, __u32)   /* id for prog_ifindex / map_ifindex */
```

The id is allocated by the generic offload registry ([P2](../kernel/p2-offload.md))
when apmu.ko registers, and is also readable from
`/sys/class/misc/apmu/offload_id`. libbpf sets it with
`bpf_program__set_ifindex()` and `bpf_map__set_ifindex()`.

### Install

```c
enum apmu_ev_class { APMU_EV_CORE = 1, APMU_EV_LLC, APMU_EV_DRAM };
enum apmu_ev       { APMU_EV_RD_REQ = 1, APMU_EV_WR_REQ, APMU_EV_RD_RES, APMU_EV_WR_RES,
                     APMU_EV_CORE_SLOT0 = 16 /* .. +3: CVA6 EVU slots */ };
enum apmu_op       { APMU_OP_COUNT, APMU_OP_ADD, APMU_OP_MAX,
                     APMU_OP_IN_RANGE, APMU_OP_GT, APMU_OP_LT };
enum apmu_scope    { APMU_SCOPE_TASK = 1, APMU_SCOPE_CORE, APMU_SCOPE_SYSTEM };

struct apmu_event_req {
    __u8  cls, event, op, scope;
    __u8  core;             /* APMU_SCOPE_CORE: which core */
    __u8  info_lo, info_hi; /* slice of the event info for ADD/MAX/compares */
    __u8  pad;
    __u16 val_lo, val_hi;   /* compare constants (4 bits in this RTL) */
    __u16 pad2;
};

struct apmu_manifest {
    __u32 nslots;                     /* ≤ 8 and ≤ the user's quota */
    struct apmu_event_req slot[8];
    __u32 notify_overflow;            /* bit k: overflow of slot k → POLLPRI */
    __u32 budget_us;                  /* longest program run */
};

struct apmu_install2 {
    __s32 prog_fd[4];                 /* init, exit, event, request; -1 = none */
    __u32 flags;                      /* APMU_INSTALL_PERSIST (admin) */
    struct apmu_manifest manifest;
    __u64 handle;                     /* out: opaque */
    char  err[128];                   /* out: why it failed */
};
#define APMU_IOC_INSTALL2  _IOWR('A', 17, struct apmu_install2)
```

Checks, in order: every fd is a `BPF_PROG_TYPE_APMU` program bound to this
device with the matching attach type; all maps the programs use are bound
to this device and not used by another installed component; scopes allowed;
quotas; each program's `max_path_cycles` fits `budget_us` and the
administrator's cap; space. Then translate, bind and install.

### Handles and messages

```c
struct apmu_msg2 {
    __u64 handle;
    __u32 nwords;
    __s32 timeout_ms;               /* RECV only */
    __u32 tag;                      /* RECV: 0 = notification from an event program */
    __u32 pad;
    __u32 w[64];
};
#define APMU_IOC_UNINSTALL2 _IOW('A', 18, __u64)
#define APMU_IOC_SEND2      _IOW('A', 19, struct apmu_msg2)
#define APMU_IOC_RECV2      _IOWR('A', 20, struct apmu_msg2)
```

A handle encodes the slot and generation in a way userspace must not rely
on; a stale handle fails with `ESTALE`. `poll()` reports `POLLIN` when a reply
or notification is waiting for the session, `POLLPRI` for an overflow
notification.

### Information

```c
struct apmu_info2 {
    __u32 os_ready, hung, restarts, abi_version;
    __u32 free_slots, free_ispm, free_dspm, free_counters;   /* totals only */
    __u32 nhandles;
    struct { __u64 handle; __u32 state, faults, ispm, dspm; } own[9];
};
#define APMU_IOC_INFO2  _IOR('A', 21, struct apmu_info2)
```

Other sessions' components are not listed.

### New errors

| Error | Meaning |
|---|---|
| `EACCES` | scope not allowed for the caller |
| `EDQUOT` | quota exceeded (slots, bytes, counters, in-flight) |
| `ESTALE` | handle from an earlier generation |
| `E2BIG` | program too long for the budget or the ISPM |
| `EXDEV` | program or map not bound to this APMU |

### libapmu v2

```c
int  apmu_devid(int fd);                                   /* for libbpf ifindex */
int  apmu_install_obj(int fd, struct bpf_object *obj,
                      const struct apmu_manifest *m, uint64_t *handle,
                      char *err, size_t errlen);           /* finds the 4 sections */
int  apmu_send2(int fd, uint64_t h, const uint32_t *w, unsigned n);
int  apmu_recv2(int fd, uint64_t h, uint32_t *w, unsigned max, int timeout_ms,
                uint32_t *tag);
int  apmu_call2(int fd, uint64_t h, const uint32_t *req, unsigned n,
                uint32_t *resp, unsigned max, int timeout_ms);
int  apmu_uninstall2(int fd, uint64_t h);
```

### Migration

v1 ioctls stay for root while v2 is brought up; v1 `INSTALL` of native RV32
objects requires `CAP_SYS_ADMIN` once v2 exists and is removed after.
