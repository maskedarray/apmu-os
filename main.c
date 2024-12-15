// !! TOOLCHAIN is at /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-gcc

#include <stddef.h>
#include <stdint.h>
#include <events.h>
#include <debug_printf.h>
#include <timer.h>
#include <base_component.h>
#include <queue_lib.h>
#include <string.h>
#include <latency_regulation.h>
#include <latency_binning.h>


volatile int sim_events[] = {0b1000, 0b0100 , 0b0001, 0b0010, 0b0100, 0b0010, 0b0010, 0b0010, 0b0001, 0b0001, 0b0001, 0b0001, 0b0010};
int sim_event_counter = 0;
int sim_event_max = 2;


void handle_event0(uint32_t bitmap) {
    // Print the result using printf
    printf("handler0: Hello, RISCV!\n");
    
    return;
}

void handle_req0(uint32_t* bitmap) {
    // Print the result using printf
    printf("handler0: Hello, RISCV!\n");
    
    return;
}

uint32_t wfp(void){
    if (sim_event_counter < sim_event_max) {
        printf("sending event %d\n",sim_events[sim_event_counter]);
        return sim_events[sim_event_counter++];
    } else {
        while(1){}
        return 0b0000;
    }
}

uint32_t wait_for_pending(uint32_t bitmask) {
    uint32_t ret;
    asm volatile (
        "cnt.wfp %0, %1"       // Assembly instruction
        : "=r" (ret)           // Output operand, assigning to 'ret'
        : "r" (bitmask)        // Input operand, using 'bitmask' as rs1
    );
    return ret;
}
void handle_event3() {

    printf("event handler 3\n");
    
    return;
    // Logic for handling event 0
}

void handle_request3(request_t * req){
    printf("request_handler3\n");
}


volatile uint32_t time_end;
volatile uint32_t time_start;
int main() {
    time_start = get_cycles_lo();
    init_print_mem();
    
    
    // printf("PMU Started!\n");
#ifdef DEMO_LATENCY_REGULATION
    latency_regulation();
    while(1){}
#elif defined(DEMO_LATENCY_BINNING)
    latency_binning_init();
    
    // while(1){ latency_binning();}


// #ifdef TEST_DUMMY_DYNAMIC_FUNCTIONS
//     // Following is the test for dynamic function loading
//     // Separate bin files are needed to be loaded through restore file.bin binary <addr>
//     // Currently the decided address is 0x81000000 for text containing the four functions
//     // at addresses hardcoded below. And for the respective data the address is 0x81800000
//     // Currently the files are included in git
//     typedef int (*func_ptr_t)(void);
//     func_ptr_t my_function = (func_ptr_t)0x81000000;
//     int return_value = my_function();
//     printf("Return value: %d\n", return_value);     // value is 914

//     my_function = (func_ptr_t)0x81000098;
//     return_value = my_function();
//     printf("Return value: %d\n", return_value);     // value is 2506294
    
//     my_function = (func_ptr_t)0x81000144;
//     return_value = my_function();
//     printf("Return value: %d\n", return_value);     // value is 1856

//     my_function = (func_ptr_t)0x810001f8;
//     return_value = my_function();
//     printf("Return value: %d\n", return_value);     // value is 10256
//     // End code for testing functions dynamically
// #endif

    base_component_init();
    
    WRITE_MEM(QUEUE_HEAD_ADDR, QUEUE_START_ADDR);
    WRITE_MEM(QUEUE_TAIL_ADDR, QUEUE_START_ADDR);
    
    
    init_event_handlers();
    register_event_handler(1, 1, latency_binning);       // This is a dummy event
    // register_event_handler(1, BASE_COMPONENT_BITMASK, base_component_event_handler);

    // Infinite loop
    while(1){
        uint32_t bitmap = wait_for_pending(1);
        process_events(bitmap);
    }
    // Below lines will not be executed ever
    // printf("finished running loop\n");
    // write_to_memory(0xefef);
    while(1){}
#endif
    time_end = get_cycles_lo();
    printf("print memory init time: %d\n", time_end - time_start);
    WRITE_MEM(QUEUE_HEAD_ADDR, QUEUE_START_ADDR);
    WRITE_MEM(QUEUE_TAIL_ADDR, QUEUE_START_ADDR);
    time_start = get_cycles_lo();
    base_component_init();

    


    init_event_handlers();
    time_end = get_cycles_lo();
    printf("event handler init time: %d\n", time_end - time_start);

    time_start = get_cycles_lo();
    register_event_handler(0, 8, base_component_event_handler);       // This is a dummy event
    register_event_handler(1, 1, handle_event0);       // This is a dummy event
    register_event_handler(2, 4, handle_event0);       // This is a dummy event
    // register_event_handler(3, 4, handle_event0);       // This is a dummy event
    register_event_handler(4, 4, handle_event0);       // This is a dummy event
    register_event_handler(5, 4, handle_event0);       // This is a dummy event
    register_event_handler(6, 4, handle_event0);       // This is a dummy event
    register_event_handler(7, 4, handle_event0);       // This is a dummy event
    register_event_handler(8, 4, handle_event0);       // This is a dummy event
    register_event_handler(9, 4, handle_event0);       // This is a dummy event
    time_end = get_cycles_lo();
    printf("9 event register time: %d\n", time_end - time_start);
    time_start = get_cycles_lo();
    base_component_request_register(0,handle_req0);
    base_component_request_register(1,handle_req0);
    base_component_request_register(2,handle_req0);
    // base_component_request_register(3,handle_req0);
    base_component_request_register(4,handle_req0);
    base_component_request_register(5,handle_req0);
    base_component_request_register(6,handle_req0);
    base_component_request_register(7,handle_req0);
    base_component_request_register(8,handle_req0);
    base_component_request_register(9,handle_req0);
    time_end = get_cycles_lo();
    printf("9 d base component register: %d\n", time_end - time_start);

    // time_start = get_cycles_lo();
    // uint32_t buff = put_request("Hello World! \n", 16, 6);
    // time_end = get_cycles_lo();
    // printf("request buffer time: %d\n", time_end - time_start);

    // memcpy((void*)buff, "Hello world\n", 13);

    // time_start = get_cycles_lo();
    // put_request_push();
    // time_end = get_cycles_lo();
    // printf("request push time: %d\n", time_end - time_start);

    // uint32_t size_out;
    // time_start = get_cycles_lo();
    // volatile uint32_t* ptr = get_request(&size_out);
    // time_end = get_cycles_lo();
    // printf("request get buffer time: %d\n", time_end - time_start);

    // time_start = get_cycles_lo();
    // get_request_pop();
    // time_end = get_cycles_lo();
    // printf("request get pop time: %d\n", time_end - time_start);


    request_comp_installer_t new_req;
    new_req.bitmask = 0b0010;
    new_req.event_handler = handle_event3;
    new_req.id = 3;
    new_req.request_handler = handle_request3;
    uint32_t request_id = REQUEST_INSTALL_ID;
    
    time_start = get_cycles_lo();
    uint32_t buff = put_request(&new_req, sizeof(new_req),request_id);
    time_end = get_cycles_lo();
    printf("install request put buffer get: %d\n", time_end - time_start);

    memcpy((void*)buff, &new_req, sizeof(request_comp_installer_t));

    time_start = get_cycles_lo();
    put_request_push();
    time_end = get_cycles_lo();
    printf("install request put buffer push: %d\n", time_end - time_start);


    request_comp_installer_t new_req2;
    new_req2.bitmask = 0b0010;
    new_req2.event_handler = handle_event3;
    new_req2.id = 3;
    new_req2.request_handler = handle_request3;
    uint32_t request_id2 = REQUEST_UNINSTALL_ID;
    buff = put_request(&new_req2, sizeof(new_req2),request_id2);
    memcpy((void*)buff, &new_req2, sizeof(request_comp_installer_t));
    put_request_push();
  
    while(1){
        uint32_t bitmap = wfp();
        time_start = get_cycles_lo();
        process_events(bitmap);
        time_end = get_cycles_lo();
        printf("base component request handler call time: %d\n", time_end - time_start);
    }


    base_component_exit();// This should never be reached
    while (1);
    return 0;  // This should never be reached
}


