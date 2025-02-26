#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "queue_lib.h"

void delay(unsigned int count) {
    while (count--) {
        // Simple delay loop
        asm volatile("nop");
    }
}



void main() {
    volatile char *mem1 = (volatile char *)0x82000000;
    volatile char *mem2 = (volatile char *)0x83000000;

    delay(100000000);
    
    consume_requests();

    if (*mem1 == 'A' && *mem2 == 'B') {
            volatile char *uart = (volatile char *)0x10000000;
            const char *msg = "Core1: Success!\n";
            while (*msg)
            {
                *uart = *msg++;
            }
    }

    while(1);
}
