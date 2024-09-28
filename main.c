// !! TOOLCHAIN is at /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-gcc

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <events.h>
#include <debug_printf.h>
#include <timer.h>
#include <base_component.h>
#include <queue_lib.h>
#include <string.h>

int sim_events[] = {0b0100, 0b0010 , 0b0001, 0b0100, 0b0100, 0b0010, 0b0010, 0b0100, 0b0001, 0b0001, 0b0001, 0b0001, 0b0010};
int sim_event_counter = 0;
int sim_event_max = 13;

// Example event handler functions (replace with actual functions)
void handle_event0() {
    // printf("Handler for event 0\n");
    delay_time_us(1000000);
    uint64_t mcycle_value = get_cycles(); // Get the 64-bit cycle count

    // Buffer to hold the formatted string
    char buffer[50]; // Ensure it's large enough to hold the string

    // Generate the string
    sprintf(buffer, "handler0: Hello, RISC-V! %llu\n", mcycle_value); // Use %llu for 64-bit unsigned

    // Print the result using printf
    printf("%s", buffer);
    
    return;
    // Logic for handling event 0
}


typedef struct {
    uint32_t size;
    char* payload[128];
} componend3_request_t;
// Example event handler functions (replace with actual functions)
void handle_event3() {
    // printf("Handler for event 0\n");
    delay_time_us(1000000);

    // Buffer to hold the formatted string
    char buffer[100]; // Ensure it's large enough to hold the string

    // Generate the string
    sprintf(buffer, "handler3: This is the event handler for component 3\n"); // Use %llu for 64-bit unsigned

    // Print the result using printf
    printf("%s", buffer);
    
    return;
    // Logic for handling event 0
}

void handle_request3(request_t * req){
    printf("request_handler3: This is request handler for component 3\n");
}

// Prototype for the wfp() function (returns 32-bit event register)
uint32_t wfp(void){
    return (sim_event_counter < sim_event_max)? sim_events[sim_event_counter++] : 0b0000;
}



int main() {
    
    // Print the result using printf
    printf("PMU Core Started!\n");

#ifdef TEST_DUMMY_DYNAMIC_FUNCTIONS
    // Following is the test for dynamic function loading
    // Separate bin files are needed to be loaded through restore file.bin binary <addr>
    // Currently the decided address is 0x81000000 for text containing the four functions
    // at addresses hardcoded below. And for the respective data the address is 0x81800000
    // Currently the files are included in git
    typedef int (*func_ptr_t)(void);
    func_ptr_t my_function = (func_ptr_t)0x81000000;
    int return_value = my_function();
    printf("Return value: %d\n", return_value);     // value is 914

    my_function = (func_ptr_t)0x81000098;
    return_value = my_function();
    printf("Return value: %d\n", return_value);     // value is 2506294
    
    my_function = (func_ptr_t)0x81000144;
    return_value = my_function();
    printf("Return value: %d\n", return_value);     // value is 1856

    my_function = (func_ptr_t)0x810001f8;
    return_value = my_function();
    printf("Return value: %d\n", return_value);     // value is 10256
    // End code for testing functions dynamically
#endif

    base_component_init();

    WRITE_MEM(HEAD_ADDR, QUEUE_START_ADDR);
    WRITE_MEM(TAIL_ADDR, QUEUE_START_ADDR);

    // generate request to install component with ID 3
    request_comp_installer_t new_req;
    new_req.bitmask = 0b0010;
    new_req.event_handler = handle_event3;
    new_req.id = 3;
    new_req.request_handler = handle_request3;
    uint32_t request_id = REQUEST_INSTALL_ID;
    while (put_request(&new_req, sizeof(new_req),request_id) == PUT_FAIL){

    }

    // generate request to be passed to request handler of component with ID 3
    componend3_request_t comp3_req;
    strcpy(comp3_req.payload, "This is request handler for newly installed component 3\n");
    comp3_req.size = strlen(comp3_req.payload);
    request_id = 3;
    while (put_request(&comp3_req, sizeof(comp3_req),request_id) == PUT_FAIL){

    }


    // generate request to uninstall component with ID 3
    new_req.bitmask = 0b0010;
    new_req.event_handler = handle_event3;
    new_req.id = 3;
    new_req.request_handler = handle_request3;
    request_id = REQUEST_UNINSTALL_ID;
    while (put_request(&new_req, sizeof(new_req),request_id) == PUT_FAIL){

    }
    
    
    init_event_handlers();
    register_event_handler(2, 0b1100, handle_event0);       // This is a dummy event
    register_event_handler(1, BASE_COMPONENT_BITMASK, base_component_event_handler);

    // Infinite loop
    while(1){
        uint32_t bitmap = wfp();
        process_events(bitmap);
    }

    base_component_exit();// This should never be reached
    while (1);
    return 0;  // This should never be reached
}


