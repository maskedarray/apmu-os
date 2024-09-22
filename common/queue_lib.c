#include "queue_lib.h"


// Function to add a new request object to the queue (producer)
int put_request(void* req_ptr, uint32_t size, int reqnum) {
    if (size == 0) return PUT_FAIL;
    uint32_t head = READ_MEM(HEAD_ADDR);  // Get current head pointer
    uint32_t next_free_addr = head;
    uint32_t tail = READ_MEM(TAIL_ADDR);
    if (((request_t*)head)->size != 0) { // If always taken except first initialization
        next_free_addr = head + sizeof(request_t) + ((request_t*)head)->size;
        if(next_free_addr >= (QUEUE_SIZE + QUEUE_START_ADDR)){       // current head points to a wraped around object
            next_free_addr -= (QUEUE_SIZE); 
            if ((next_free_addr + sizeof(request_t) + size) >= tail)
                return PUT_FAIL;  
        }
    }

    uint32_t total_size = sizeof(request_t) + size;
    
    // This loop will never be triggered if head points to object that is wrapped around.
    // This assumption if proven wrong will render the following logic faulty
    if (next_free_addr + total_size > QUEUE_SIZE + QUEUE_START_ADDR) {
        
        // Calculate remaining space before wrapping around
        uint32_t space_before_wrap = (QUEUE_SIZE + QUEUE_START_ADDR) - next_free_addr;
        if (space_before_wrap > sizeof(request_t) + 1){
            // Not enough space before wrap, check tail pointer
            // first condition should never be true That means that tail data has already been overwritten
            uint32_t end_addr = QUEUE_START_ADDR + total_size - space_before_wrap;
            if (tail <= end_addr) {    // no object should be equal or more than half of total queue size
                // If tail pointer is between next_free_addr and overflow, reject the request
                return PUT_FAIL;
            }

            // Create new request at next_free_addr
            request_t* req = (request_t*)next_free_addr;
            req->next = 0; // No next request yet
            req->size = size;
            req->consumed = 0;
            req->req_id = reqnum;

            // Copy part of the payload that fits before the wrap
            memcpy(req->payload, req_ptr, space_before_wrap - sizeof(request_t));

            // Step 2: Wrap around and copy the remaining payload at the start of the queue
            req_ptr = (char*)req_ptr + (space_before_wrap - sizeof(request_t));
            uint32_t remaining_size = size - (space_before_wrap - sizeof(request_t));

            uint32_t next_free_addr2 = QUEUE_START_ADDR;  // Wrap to the start of the queue
            memcpy((void*)next_free_addr2, req_ptr, remaining_size);
            request_t* last_req = (request_t*)head;
            last_req->next = next_free_addr;  // Link last request to new one

            WRITE_MEM(HEAD_ADDR, next_free_addr);
        } else{
            // Not enough space before wrap, check tail pointer
            // first condition should never be true That means that tail data has already been overwritten
            uint32_t end_addr = QUEUE_START_ADDR + total_size ;
            if (tail <= end_addr) {    // no object should be equal or more than half of total queue size
                // If tail pointer is between next_free_addr and overflow, reject the request
                return PUT_FAIL;
            }

            // Create new request at next_free_addr
            request_t* req = (request_t*)QUEUE_START_ADDR;
            req->next = 0; // No next request yet
            req->size = size;
            req->consumed = 0;
            req->req_id = reqnum;
            space_before_wrap = 0;
            next_free_addr = QUEUE_START_ADDR;
            // // Copy part of the payload that fits before the wrap
            // memcpy(req->payload, req_ptr, space_before_wrap - sizeof(request_t));

            // Step 2: Wrap around and copy the remaining payload at the start of the queue
            uint32_t remaining_size = size;

            uint32_t payload_addr = QUEUE_START_ADDR + sizeof(request_t);  // Wrap to the start of the queue
            memcpy((void*)payload_addr, req_ptr, remaining_size);
            request_t* last_req = (request_t*)head;
            last_req->next = next_free_addr;  // Link last request to new one

            WRITE_MEM(HEAD_ADDR, next_free_addr);
        }

        
        

    } else if (head < tail){
        if ((next_free_addr +sizeof(request_t) + size) >= tail){
            return PUT_FAIL;
        }
        
                // Create new request at next_free_addr
        request_t* req = (request_t*)next_free_addr;
        req->next = 0; // No next request yet
        req->size = size;
        req->consumed = 0;
        req->req_id = reqnum;
        // Copy the payload
        memcpy(req->payload, req_ptr, size);
        
        request_t* last_req = (request_t*)head;
        last_req->next = next_free_addr;  // Link last request to new one
        WRITE_MEM(HEAD_ADDR, next_free_addr);
    } 
    else {

    
        // Create new request at next_free_addr
        request_t* req = (request_t*)next_free_addr;
        req->next = 0; // No next request yet
        req->size = size;
        req->consumed = 0;
        req->req_id = reqnum;
        // Copy the payload
        memcpy(req->payload, req_ptr, size);
        
        request_t* last_req = (request_t*)head;
        last_req->next = next_free_addr;  // Link last request to new one
        WRITE_MEM(HEAD_ADDR, next_free_addr);
    }
    return PUT_SUCCESS;
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
        
        

        
        // Return pointer to the request
        return req;
    } else if (req->consumed == 1){       
        if (req->next != 0) {
            // TODO: it should return request pointer  in this path
            WRITE_MEM(TAIL_ADDR, req->next);  // Move to the next request
        }
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

            if ( ((char *)request_payload + sizeof(request_t) + request_payload->size) >  (QUEUE_SIZE+QUEUE_START_ADDR) ){ // wrap around condition
                uint32_t bytes_before_wrap = QUEUE_SIZE + QUEUE_START_ADDR - (uint32_t)(request_payload->payload);
                uint32_t bytes_after_wrap = request_payload->size - bytes_before_wrap;

                // Print the first part of the payload (before wrap) over UART
                for (uint32_t i = 0; i < bytes_before_wrap; i++) {
                    *uart = request_payload->payload[i];
                }

                // Now print the remaining part of the payload that wrapped around
                volatile char *wrapped_payload = (volatile char *)QUEUE_START_ADDR;  // Start address after wrap
                for (uint32_t i = 0; i < bytes_after_wrap; i++) {
                    *uart = wrapped_payload[i];
                }

                const char *newline = "\n";
                while (*newline) {
                    *uart = *newline++;
                }

                // Mark the request as consumed
                request_payload->consumed = 1;
                // Move the tail to the next request (or keep same if next = 0)
                if (request_payload->next != 0) {
                    WRITE_MEM(TAIL_ADDR, request_payload->next);  // Move to the next request
                }
            } else {

                // Print the payload (request data) over UART
                for (uint32_t i = 0; i < size; i++) {
                    *uart = request_payload->payload[i];
                }
                
                const char *newline = "\n";
                while (*newline) {
                    *uart = *newline++;
                }

                request_payload->consumed = 1;
                // Move the tail to the next request (or keep same if next = 0)
                if (request_payload->next != 0) {
                    WRITE_MEM(TAIL_ADDR, request_payload->next);  // Move to the next request
                }
            }
        }
        
        
    }
}
// TODO: handle the corner case when sieze is greater than max_size /2
