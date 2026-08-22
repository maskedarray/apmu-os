#ifndef __COMMON_DEFINES_H__
#define __COMMON_DEFINES_H__

#include <pmu_hw_desc.h>

// Macros for 32-bit read-write to memory address.
#define read_32b(addr)         (*(volatile int *)(long)(addr))
#define write_32b(addr, val_)  (*(volatile int *)(long)(addr) = val_)

// The linker script places .rodata/.data/.bss/.stack in the first 0x1000 of
// DSPM. The queue and the debug-print buffer live immediately above it.
// Expressed relative to DSPM_BASE_ADDR so they follow the map automatically.

// Macros for debug printf to memory
#define PRINT_START_ADDRESS ((volatile char *)(DSPM_BASE_ADDR + 0x1800))
#define PRINT_END_ADDRESS   ((volatile char *)(DSPM_BASE_ADDR + 0x1FFC))

// Single-word scratch slot used by write_to_memory() for ad-hoc debugging.
#define DEBUG_SCRATCH_ADDR  (DSPM_BASE_ADDR + 0xFFC)

// Macros for PMU Receiver Queue
#define QUEUE_HEAD_ADDR  (DSPM_BASE_ADDR + 0x1000)  // Address of head pointer (producer)
#define QUEUE_TAIL_ADDR  (DSPM_BASE_ADDR + 0x1004)  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR (DSPM_BASE_ADDR + 0x1008)  // Start of the queue
#define QUEUE_SIZE 0x3F8  // Define the queue size minus head ADDR and Tail ADDR
// QUEUE Size currently ~ 1K bytes

#endif