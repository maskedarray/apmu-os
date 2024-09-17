
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "queue_lib.h"



void put_multiple_requests() {
    // UART address for printing messages
    volatile char *uart = (volatile char *)0x10000000;
    
    // Payload data for requests
    char payload1[] = "Request 1";
    char payload2[] = "Request 2";
    char payload3[] = "Request 3";
    char payload4[] = "Request 4";
    char payload5[] = "Request 5";
    
    // Put 5 requests into the queue
    put_request(payload1, sizeof(payload1));
    put_request(payload2, sizeof(payload2));
    put_request(payload3, sizeof(payload3));
    put_request(payload4, sizeof(payload4));
    put_request(payload5, sizeof(payload5));
    
    // Print success message to UART
    const char *msg = "5 requests placed into the queue.\n";
    while (*msg) {
        *uart = *msg++;
    }
    
    while (1);  // Keep core active
}

void main()
{
    // volatile char *mem1 = (volatile char *)0x82000000;
    // volatile char *mem2 = (volatile char *)0x83000000;

    // *mem1 = 'A';
    // *mem2 = 'B';

    WRITE_MEM(HEAD_ADDR, QUEUE_START_ADDR);
    WRITE_MEM(TAIL_ADDR, QUEUE_START_ADDR);


    put_multiple_requests();

    volatile char *uart = (volatile char *)0x10000000;
    const char *msg = "Hello, RISC-V!\n";
    while (*msg)
    {
        *uart = *msg++;
    }
    while (1)
        ;
}
