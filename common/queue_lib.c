#include "queue_lib.h"


// Function to add a new request object to the queue (producer)
void put_request(void* req_ptr, uint32_t size) {
    if (size == 0) return;
    uint32_t head = READ_MEM(HEAD_ADDR);  // Get current head pointer
    uint32_t next_free_addr = head;
    if (((request_t*)head)->size != 0) { // It will only go to else on the first initialization
        next_free_addr = head + sizeof(request_t) + ((request_t*)head)->size;
        if(next_free_addr >= (QUEUE_SIZE + QUEUE_START_ADDR)){       // current head points to a wraped around object
            next_free_addr -= (QUEUE_SIZE + QUEUE_START_ADDR);   
        }
    }

    uint32_t total_size = sizeof(request_t) + size;
    uint32_t tail = READ_MEM(TAIL_ADDR);
    if (next_free_addr + total_size > QUEUE_SIZE + QUEUE_START_ADDR) {
        // Calculate remaining space before wrapping around
        uint32_t space_before_wrap = (QUEUE_SIZE + QUEUE_START_ADDR) - next_free_addr;
    
        // Not enough space before wrap, check tail pointer
        // first condition should never be true That means that tail data has already been overwritten
        if (tail <= next_free_addr || tail <= next_free_addr + total_size) {    // no object should be equal or more than half of total queue size
            // If tail pointer is between next_free_addr and overflow, reject the request
            return;
        }

        // Create new request at next_free_addr
        request_t* req = (request_t*)next_free_addr;
        req->next = 0; // No next request yet
        req->size = size;
        req->consumed = 0;

        // Copy part of the payload that fits before the wrap
        memcpy(req->payload, req_ptr, space_before_wrap - sizeof(request_t));

        // Step 2: Wrap around and copy the remaining payload at the start of the queue
        req_ptr = (char*)req_ptr + (space_before_wrap - sizeof(request_t));
        uint32_t remaining_size = size - (space_before_wrap - sizeof(request_t));

        uint32_t next_free_addr2 = QUEUE_START_ADDR;  // Wrap to the start of the queue
        memcpy((void*)next_free_addr2, req_ptr, remaining_size);
        WRITE_MEM(HEAD_ADDR, next_free_addr);
        

    } else {

    
        // Create new request at next_free_addr
        request_t* req = (request_t*)next_free_addr;
        req->next = 0; // No next request yet
        req->size = size;
        req->consumed = 0;
        
        // Copy the payload
        memcpy(req->payload, req_ptr, size);
        
        request_t* last_req = (request_t*)head;
        last_req->next = next_free_addr;  // Link last request to new one
        WRITE_MEM(HEAD_ADDR, next_free_addr);
    }
}



// Function to get the next request (consumer)
void* get_request(uint32_t* size_out) {
    // This function is utilized by the pmu core and the hypervisor is 
    // responsible for initializing the request queue and then starting 
    // the PMU core. Thus the check for tail pointer zero is not required
    // as the hypervisor will change the tail pointer to the start address. 
    // However, the tail pointer being at the start address may see the 
    // consumed is zero and size is zero, and thus will not read the data
    // and will return NULL
    //
    //
    // uint32_t tail = read_mem(TAIL_ADDR);  // Get current tail pointer
    // if (tail == 0) return NULL;  // Not initialized yet
    //
    //

    uint32_t tail = READ_MEM(TAIL_ADDR);  // Get current tail pointer

    request_t* req = (request_t*)tail;  // Get the current request
    if (req->consumed == 0 && req->size != 0){  // consumed and size will be zero for first request object only
        // Extract the payload size
        *size_out = req->size;
        
        // Move the tail to the next request (or keep same if next = 0)
        if (req->next != 0) {
            WRITE_MEM(TAIL_ADDR, req->next);  // Move to the next request
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