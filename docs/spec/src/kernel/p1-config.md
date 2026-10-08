# P1: configuration

<span class="st prop">PROPOSED</span>

## Before

```text
# CONFIG_BPF_SYSCALL is not set
# CONFIG_NET is not set
```

## After

```text
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_UNPRIV_DEFAULT_OFF=y      # unprivileged bpf() stays off, except what P3 allows
# CONFIG_BPF_JIT is not set          # nothing runs on the host
# CONFIG_NET is not set              # unchanged: P2 removes the dependency
CONFIG_STRICT_DEVMEM=y               # with IO_STRICT_DEVMEM: /dev/mem cannot reach
CONFIG_IO_STRICT_DEVMEM=y            #   the APMU once apmu.ko claims it
CONFIG_APMU_HOOKS=y                  # P4
```

Carried as a fragment in `alsaqr-software/linux/configs/` and merged by the
buildroot config. Check after the build: the kernel's `_end` stays below
`0x81800000` (see `alsaqr-software/linux/CLAUDE.md`).

## Why no JIT and no networking

- Offloaded programs never run on the host, so the host JIT and interpreter
  are irrelevant to them. Without `CONFIG_BPF_JIT` the kernel still has the
  interpreter for any other program type a privileged user loads.
- Enabling `CONFIG_NET` only to satisfy offload's dependency would add the
  network stack to a kernel that has none, and would need a fake network
  device for the APMU. P2 removes the dependency instead.
