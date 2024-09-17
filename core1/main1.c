#include <stdint.h>
#include <stddef.h>
#include <string.h>

void delay(unsigned int count) {
    while (count--) {
        // Simple delay loop
        asm volatile("nop");
    }
}


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

// Function to get the next request (consumer)
void* get_request(uint32_t* size_out) {
    uint32_t tail = read_mem(TAIL_ADDR);  // Get current tail pointer
    if (tail == 0) return NULL;  // Not initialized yet
    
    request_t* req = (request_t*)tail;  // Get the current request
    if (req->consumed == 0){
        // Extract the payload size
        *size_out = req->size;
        
        // Move the tail to the next request (or NULL if no more requests)
        if (req->next != 0) {
            write_mem(TAIL_ADDR, req->next);  // Move to the next request
        }

        
        // Return pointer to the request
        return req;
    } else {
        return NULL;
    }
}



void consume_requests() {
    // UART address for printing messages
    volatile char *uart = (volatile char *)0x10000000;
    
    while (1) {
        uint32_t size;
        request_t *request_payload = get_request(&size);  // Check for a new request
        
        if (request_payload != NULL) {
            // Print the payload (request data) over UART
            for (uint32_t i = 0; i < size; i++) {
                *uart = request_payload->payload[i];
            }
            
            const char *newline = "\n";
            while (*newline) {
                *uart = *newline++;
            }

            request_payload->consumed = 1;
        }
        
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
