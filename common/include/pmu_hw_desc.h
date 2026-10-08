#ifndef __PMU_HW_DESC_H__
#define __PMU_HW_DESC_H__

// Macros for custom PMU core instructions.
// Counter read: rd = cnt[idx] (bits 30:0)
#define counter_read(rd, idx)       asm volatile ("cnt.rd\t%0,%1" : "=r" (rd) : "r" (idx))
// Counter write: cnt[idx] = val. The assembler takes the value first (rs2, rs1).
#define counter_write(idx, val)     asm volatile ("cnt.wr\t%1,%0" :: "r" (idx), "r" (val))
// Wait for pending: blocks until a counter in mask is pending, fired = those counters.
#define counter_wait_pending(fired, mask) \
    asm volatile ("cnt.wfp\t%0,%1" : "=r" (fired) : "r" (mask) : "memory")


// #############################################################################
// Defines for PMU
// #############################################################################

#define NUM_COUNTER 32

#define TIMER_WIDTH     0x8
#define STATUS_WIDTH    0x4
#define BOOT_ADDR_WIDTH 0x4
#define COUNTER_WIDTH   0x4

// PMU Bundle Addresses
#define PMU_B_BASE_ADDR     0x10405000
#define TIMER_ADDR          PMU_B_BASE_ADDR
#define PERIOD_ADDR         (PMU_B_BASE_ADDR + 0x8)
#define PMC_STATUS_ADDR     (PMU_B_BASE_ADDR + 0x1000)
#define PMC_BOOT_ADDR       (PMU_B_BASE_ADDR + 0x1004)
// Two 64-bit (8B) timer and one 32-bit status registers in the PMU bundle.
#define PMU_BUNDLE_SIZE     0x2000

// Counter Bundle Base Addresses
#define COUNTER_B_BASE_ADDR     (PMU_B_BASE_ADDR + PMU_BUNDLE_SIZE)
#define COUNTER_BASE_ADDR       (COUNTER_B_BASE_ADDR + 0*COUNTER_WIDTH)
#define EVENT_SEL_BASE_ADDR     (COUNTER_B_BASE_ADDR + 1*COUNTER_WIDTH)
#define EVENT_INFO_BASE_ADDR    (COUNTER_B_BASE_ADDR + 2*COUNTER_WIDTH)
#define INIT_BUDGET_BASE_ADDR   (COUNTER_B_BASE_ADDR + 3*COUNTER_WIDTH)
// Four 32-bit (4B) registers in one counter bundle.
#define COUNTER_BUNDLE_SIZE     0x1000
#define COUNTER_ADDR(i)         (COUNTER_BASE_ADDR + (i) * COUNTER_BUNDLE_SIZE)
#define EVENT_SEL_ADDR(i)       (EVENT_SEL_BASE_ADDR + (i) * COUNTER_BUNDLE_SIZE)
#define EVENT_INFO_ADDR(i)      (EVENT_INFO_BASE_ADDR + (i) * COUNTER_BUNDLE_SIZE)

// PMU Core Addresses
#define ISPM_BASE_ADDR  0x10427000
#define DSPM_BASE_ADDR  0x10429000
#define DSPM_LENGTH     0x20000
#define DSPM_END_ADDR   (DSPM_BASE_ADDR + DSPM_LENGTH)
// Event select: port val/mask, source val/mask, event val/mask, one nibble each.
// Ports: 1-4 CVA6 EVU of core 0-3, 5-8 core 0-3 <-> LLC, 9 LLC <-> DRAM.
// Events: 1 read request, 2 write request, 3 read response, 4 write response.
// On port 9 the source is the initiating core (core 0 = source 0 verified on hardware).
#define LLC_RD_REQ_CORE(n)  (0x5F001F + ((n) << 20))
#define LLC_WR_REQ_CORE(n)  (0x5F002F + ((n) << 20))
#define LLC_RD_RES_CORE(n)  (0x5F003F + ((n) << 20))
#define LLC_WR_RES_CORE(n)  (0x5F004F + ((n) << 20))
#define MEM_RD_REQ          0x9F001F
#define MEM_WR_REQ          0x9F002F
#define MEM_RD_RES          0x9F003F
#define MEM_WR_RES          0x9F004F
#define MEM_RD_RES_CORE(n)  (0x9F0F3F + ((n) << 12))
#define MEM_WR_RES_CORE(n)  (0x9F0F4F + ((n) << 12))

// Event info. The *_RESP_LAT values need a response event selected.
#define ADD_RESP_LAT   0x8001E0
#define MAX_RESP_LAT   0x8005E0
// Only count accesses targeting the memory subsystem (LLC, main memory).
#define CNT_MEM_ONLY   0x808E10
#define OVERFLOW_EN    0x1000000

// #############################################################################

#endif
