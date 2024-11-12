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

#define DEMO_LATENCY_BINNING

int sim_events[] = {0b0100, 0b0010 , 0b0001, 0b0010, 0b0100, 0b0010, 0b0010, 0b0010, 0b0001, 0b0001, 0b0001, 0b0001, 0b0010};
int sim_event_counter = 0;
int sim_event_max = 13;

void handle_event0(uint32_t bitmap) {
    // Print the result using printf
    printf("handler0: Hello, RISCV!\n");
    
    return;
}

uint32_t wfp(void){
    if (sim_event_counter < sim_event_max) {
        printf("sending event%d\n",sim_events[sim_event_counter]);
        return sim_events[sim_event_counter++];
    } else {
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

int main() {
    
    init_print_mem();

    printf("PMU Started!\n");
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
    base_component_exit();// This should never be reached
    while (1);
    return 0;  // This should never be reached
}


