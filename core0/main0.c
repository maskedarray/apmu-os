
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define HEAD_ADDR 0x82000000  // Address of head pointer (producer)
#define TAIL_ADDR 0x82000004  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR 0x82000008  // Start of the queue


typedef struct request {
    uint32_t next;     // Pointer to next element in the queue
    uint32_t size;     // Size of the payload
    uint32_t consumed;
    char payload[];    // Payload of variable size
} request_t;


// Helper to write 4 bytes to shared memory
void write_mem(uint32_t addr, uint32_t value) {
    *((volatile uint32_t*)addr) = value;
}


// Helper to read 4 bytes from shared memory
uint32_t read_mem(uint32_t addr) {
    return *((volatile uint32_t*)addr);
}

// Function to add a new request object to the queue (producer)
void put_request(void* req_ptr, uint32_t size) {
    uint32_t head = read_mem(HEAD_ADDR);  // Get current head pointer
    uint32_t next_free_addr;
    if (((request_t*)head)->size != 0)
        next_free_addr = head + sizeof(request_t) + ((request_t*)head)->size;
    else
        next_free_addr = head;
    
    // Create new request at next_free_addr
    request_t* req = (request_t*)next_free_addr;
    req->next = 0; // No next request yet
    req->size = size;
    req->consumed = 0;
    
    // Copy the payload
    memcpy(req->payload, req_ptr, size);
    
    request_t* last_req = (request_t*)head;
    last_req->next = next_free_addr;  // Link last request to new one
    write_mem(HEAD_ADDR, next_free_addr);
}

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

    write_mem(HEAD_ADDR, QUEUE_START_ADDR);
    write_mem(TAIL_ADDR, QUEUE_START_ADDR);
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
