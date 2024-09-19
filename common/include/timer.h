#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

// Function to get the current cycle count
uint64_t get_cycles(void);

// Function to create a delay in cycles
void delay_cycles(uint32_t cycles);

// Function to get the current time in microseconds
uint64_t get_time(void);

// Function to create a delay in microseconds
void delay_time(uint32_t microseconds, uint32_t cpu_frequency);

#endif // __TIMER_H__
