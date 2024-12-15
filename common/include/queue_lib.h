#ifndef QUEUE_LIB_H
#define QUEUE_LIB_H

#include <stdint.h>
#include <string.h>
#include <common_defines.h>

#define PUT_SUCCESS 0
#define PUT_FAIL 1

// Request structure
typedef struct request {
    volatile uint32_t next;     // Pointer to next element in the queue
    volatile uint32_t size;     // Size of the payload
    volatile uint32_t consumed;
    volatile uint32_t req_id;
    volatile char payload[];    // Payload of variable size
} request_t;

// Memory access macros
#define WRITE_MEM(addr, value) (*((volatile uint32_t *)(addr)) = (value))
#define READ_MEM(addr) (*((volatile uint32_t *)(addr)))

// Function prototypes
void put_request_push();
uint32_t put_request(void* req_ptr, uint32_t size, int reqnum);
void get_request_pop();
void* get_request(uint32_t* size_out);
void consume_requests(void);



#endif // QUEUE_LIB_H