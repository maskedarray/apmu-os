
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "queue_lib.h"



void put_multiple_requests() {
    // UART address for printing messages
    volatile char *uart = (volatile char *)0x10000000;
    
    // Payload data for requests
    char payload1[] = "Request 1: Initialize the system and perform a full diagnostic check of all sensors.";
    char payload2[] = "Request 2: Retrieve the latest environmental data including temperature, humidity, and pressure.";
    char payload3[] = "Request 3: Execute a firmware update on all connected devices and log the update status.firmware update on all up..";
    char payload4[] = "Request 4: Run a performance benchmark test on the network and report throughput and latency.";
    char payload5[] = "Request 5: Initiate a system shutdown sequence, ensuring that all data is safely saved before powering off.";

    
    // Put 5 requests into the queue
    put_request(payload1, sizeof(payload1),1);
    put_request(payload2, sizeof(payload2),1);
    while (put_request(payload3, sizeof(payload3),1) == PUT_FAIL){

    }
    while (put_request(payload4, sizeof(payload4),1) == PUT_FAIL){

    }
    while (put_request(payload5, sizeof(payload5),5) == PUT_FAIL){

    }
    
    // // Print success message to UART
    // const char *msg = "5 requests placed into the queue.\n";
    // while (*msg) {
    //     *uart = *msg++;
    // }
    
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
