# Usage examples

## Today <span class="st impl">IMPLEMENTED</span>

### From the shell (on the board)

```sh
apmu boot text_section.bin data_rodata_bss.bin   # root; removes all components
apmu install latency_binning.o                   # -> 1
apmu req 1 2                                     # 10 bin values
apmu req 1 3 0                                   # only reads from core 0
apmu run hello.o 5 6 7                           # -> 00000006 00000007 00000008
apmu list
apmu uninstall 1
```

### From C

```c
#include "libapmu.h"

int main(void)
{
    char err[96];
    uint32_t req[] = { 2 }, bins[APMU_MSG_WORDS];
    int fd = apmu_open();
    int id = apmu_install_file(fd, "latency_binning.o", 0, err, sizeof err);

    if (id < 0) { fprintf(stderr, "install: %s\n", err); return 1; }
    sleep(1);
    int n = apmu_call(fd, id, req, 1, bins, APMU_MSG_WORDS, 1000);
    for (int i = 0; i < n; i++)
        printf("bin %d: %u\n", i, bins[i]);
    apmu_close(fd);                     /* uninstalls it */
    return 0;
}
```

A full raw-MMIO example is in
`alsaqr-software/linux/apmu/example/apmu_hello.c`. The maintained hardware
sanity sequence is in `knowledgebase/running-apmu-os.md`.

### Building a component

```bash
cd apmu-os && source ./source.sh       # in the OrbStack VM
make components                        # build/hello.o, build/latency_binning.o
```

## Target <span class="st prop">PROPOSED</span>

### Build

```bash
clang -O2 -target bpf -mcpu=v3 -I"$APMU_UAPI" -c latency_binning.bpf.c -o latency_binning.bpf.o
```

### Load and install with libbpf and libapmu v2

```c
#include <bpf/libbpf.h>
#include "libapmu.h"

int main(void)
{
    struct apmu_manifest m = {
        .nslots = 1,
        .slot[0] = { .cls = APMU_EV_DRAM, .event = APMU_EV_RD_RES,
                     .op = APMU_OP_MAX, .info_lo = 0, .info_hi = 15,
                     .scope = APMU_SCOPE_SYSTEM },
        .budget_us = 20,
    };
    char err[128];
    uint64_t h;
    int fd = apmu_open();
    struct bpf_object *obj = bpf_object__open_file("latency_binning.bpf.o", NULL);
    struct bpf_program *p;
    struct bpf_map *map;

    /* Bind every program and map to the APMU, then let the kernel verify them. */
    bpf_object__for_each_program(p, obj)
        bpf_program__set_ifindex(p, apmu_devid(fd));
    bpf_object__for_each_map(map, obj)
        bpf_map__set_ifindex(map, apmu_devid(fd));
    if (bpf_object__load(obj))          /* verifier log on failure */
        return 1;

    if (apmu_install_obj(fd, obj, &m, &h, err, sizeof err) < 0) {
        fprintf(stderr, "install: %s\n", err);   /* e.g. "scope system needs CAP_PERFMON" */
        return 1;
    }

    /* Ask the component ... */
    uint32_t req = 2, bins[64];
    int n = apmu_call2(fd, h, &req, 1, bins, 64, 1000);

    /* ... or read its map directly: the module reads DSPM. */
    __u32 key = 0, all[10];
    bpf_map__lookup_elem(bpf_object__find_map_by_name(obj, ".bss"),
                         &key, sizeof key, all, sizeof all, 0);

    apmu_uninstall2(fd, h);
    bpf_object__close(obj);
    apmu_close(fd);
    return n < 0;
}
```

### What a rejection looks like

```text
$ ./lb_loader
libbpf: prog 'on_event': BPF program load failed: Permission denied
libbpf: prog 'on_event': -- BEGIN PROG LOAD LOG --
...
12: (85) call bpf_apmu_counter_read
apmu: R1 must be a constant slot number
-- END PROG LOAD LOG --
```

```text
$ ./lb_loader
install: program 'on_event' needs 31 us, budget is 20 us
```
