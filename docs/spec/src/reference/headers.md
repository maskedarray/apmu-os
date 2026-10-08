# Source headers

Included verbatim from the repositories at build time, so this page always
matches the code it was built from.

## `apmu_abi.h`

Host ↔ apmu-os contract. `apmu-os/common/include/apmu_abi.h`; an identical
copy is `alsaqr-software/linux/apmu/apmu_abi.h`.

```c
--8<-- "apmu-os/common/include/apmu_abi.h"
```

## `apmu_ioctl.h`

Kernel ↔ userspace interface. `alsaqr-software/linux/apmu/apmu_ioctl.h`.

```c
--8<-- "alsaqr-software/linux/apmu/apmu_ioctl.h"
```

## `libapmu.h`

```c
--8<-- "alsaqr-software/linux/apmu/libapmu.h"
```

## `apmu_component.h`

```c
--8<-- "apmu-os/common/include/apmu_component.h"
```

## `pmu_hw_desc.h`

Counter instructions and register addresses as apmu-os uses them.

```c
--8<-- "apmu-os/common/include/pmu_hw_desc.h"
```

## `apmu_link.h`

The kernel's component linker.

```c
--8<-- "alsaqr-software/linux/apmu/kmod/apmu_link.h"
```
