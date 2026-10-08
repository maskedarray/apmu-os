# apmu-os

`apmu-os` is the RV32 runtime for the AlSaqr APMU processing element. The
runtime executes from an 8 KiB ISPM at `0x10427000` and uses a 128 KiB DSPM
at `0x10429000`. Code is linked at its actual load address.

The required hardware contract is:

- an RV32IM processing element with the Zicsr extension;
- host access to ISPM and DSPM;
- live host writes to ISPM regions that are not being executed;
- `cnt.rd`, `cnt.wr`, `cnt.wfp`, and `cnt.wfo` custom instructions; and
- the counter and event-selection registers defined in
  `common/include/pmu_hw_desc.h`.

## Build

Set the toolchain variables required by the Makefile:

```sh
export LLVM_BIN=/path/to/llvm/bin
export RISCV_SYSROOT=/path/to/riscv32-unknown-elf
export NEWLIB_BUILD=/path/to/riscv32-unknown-elf/newlib
export LIBGCC_A=/path/to/rv32im/ilp32/libgcc.a

make clean
make generate-bin-files components
```

Outputs:

- `build/text_section.bin`: base runtime for ISPM.
- `build/data_rodata_bss.bin`: initialized DSPM image.
- `build/hello.o` and `build/latency_binning.o`: dynamically linked
  components.

Run `make print_sizes` or inspect `build/output.elf` to check that the base
image fits below the dynamic ISPM region.

## Host integration

A compatible host driver must implement the ABI in
`common/include/apmu_abi.h`, load the base ISPM and DSPM images, and provide
component install, call, list, uninstall, halt, and boot operations. For a
host utility exposing that interface as `apmu`, a minimal smoke test is:

```sh
apmu boot /tmp/text_section.bin /tmp/data_rodata_bss.bin
apmu run /tmp/hello.o 5 6 7
apmu install /tmp/latency_binning.o
apmu list
```

The hello response should be `00000006 00000007 00000008`.

## Runtime design

The base image initializes two single-producer/single-consumer queues in DSPM,
registers the doorbell handler, and dispatches counter events and host requests.
Dynamic components are ordinary RV32 relocatable objects. The kernel module
allocates their ISPM/DSPM ranges, resolves the exports published by the base
image, and writes them into dynamically allocated ranges while the PE keeps
running. It then sends a generation-tagged install message. The base executes
`fence.i`, registers the handlers, and acknowledges. Uninstall first quiesces
the component on the PE; only then does the kernel scrub and reuse its memory.

A host-requested halt is destructive to the current runtime session. Booting
again reinitializes the queues and runtime state, and discards every installed
component.

Queues and component state use ordinary volatile ISPM/DSPM accesses. Timeouts
and trap/session reporting allow the host to distinguish a failed runtime or
component from a request that is still in progress.

## Memory layout

| Memory | Offset | Use |
|---|---:|---|
| ISPM | `0x0000` | base image |
| ISPM | `_dyn_ispm_start` to `0x2000` | dynamic component code |
| DSPM | `0x0000–0x1000` | base data, BSS and stack |
| DSPM | `0x1000` | ABI header |
| DSPM | `0x1100`, `0x1400` | request and response queues |
| DSPM | `0x1800–0x2000` | debug print buffer |
| DSPM | `0x2000–0x10000` | dynamic component data |

The complete host/runtime contract is defined by
`common/include/apmu_abi.h`.
