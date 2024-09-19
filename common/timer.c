#include "timer.h"

// Function to get the current cycle count
uint64_t get_cycles(void) {
    uint32_t low, high;

    // Read mcycle and mcycleh
    asm volatile (
        "csrr %0, mcycle\n"
        "csrr %1, mcycleh\n"
        : "=r" (low), "=r" (high)
    );

    return ((uint64_t)high << 32) | low;  // Combine high and low parts
}

// Function to create a delay in cycles
void delay_cycles(uint32_t cycles) {
    uint64_t start = get_cycles();
    uint64_t end = start + cycles;

    while (get_cycles() < end) {
        // Busy wait
    }
}

// Function to get the current time in microseconds
uint64_t get_time(void) {
    // Assuming a CPU frequency of F_CPU Hz
    // Multiply by 1000000 to convert to microseconds
    return get_cycles(); // Placeholder for actual frequency conversion
}

// Function to create a delay in microseconds
void delay_time(uint32_t microseconds, uint32_t cpu_frequency) {
    uint32_t cycles = (cpu_frequency / 1000000) * microseconds; // Convert time to cycles
    delay_cycles(cycles); // Call delay_cycles with the calculated cycles
}
