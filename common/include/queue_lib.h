#ifndef QUEUE_LIB_H
#define QUEUE_LIB_H

#define PUT_SUCCESS 0
#define PUT_FAIL 1

#define HEAD_ADDR 0x82000000  // Address of head pointer (producer)
#define TAIL_ADDR 0x82000004  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR 0x82000008  // Start of the queue
#define QUEUE_SIZE 0x1000  // Define the queue size

#include <stdint.h>
#include <string.h>

// Memory-mapped queue addresses
#define HEAD_ADDR 0x82000000  // Address of head pointer (producer)
#define TAIL_ADDR 0x82000004  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR 0x82000008  // Start of the queue

// Request structure
typedef struct request {
    uint32_t next;     // Pointer to next element in the queue
    uint32_t size;     // Size of the payload
    uint32_t consumed;
    uint32_t req_id;
    char payload[];    // Payload of variable size
} request_t;

// Memory access macros
#define WRITE_MEM(addr, value) (*((volatile uint32_t *)(addr)) = (value))
#define READ_MEM(addr) (*((volatile uint32_t *)(addr)))

// Function prototypes
int put_request(void* req_ptr, uint32_t size, int reqnum);
void* get_request(uint32_t* size_out);
void consume_requests(void);



#endif // QUEUE_LIB_H