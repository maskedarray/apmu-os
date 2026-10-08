#ifndef __COMMON_DEFINES_H__
#define __COMMON_DEFINES_H__

#include <pmu_hw_desc.h>

// Macros for 32-bit read-write to memory address.
#define read_32b(addr)         (*(volatile int *)(long)(addr))
#define write_32b(addr, val_)  (*(volatile int *)(long)(addr) = val_)

// DSPM: 0x0000 linker region (.rodata .data .bss .stack), 0x1000 header and
// queues, 0x1800 print buffer, 0x2000 dynamic components (apmu_abi.h).

// Macros for debug printf to memory
#define PRINT_START_ADDRESS ((volatile char *)(DSPM_BASE_ADDR + 0x1800))
#define PRINT_END_ADDRESS   ((volatile char *)(DSPM_BASE_ADDR + 0x1FFC))

#endif
