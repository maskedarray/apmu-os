#include <stddef.h>
#include <stdint.h>
#include <events.h>
#include <dprintf.h>


// Example event handler functions (replace with actual functions)
void handle_event0() {
    return;
    // Logic for handling event 0
}

// Prototype for the wfp() function (returns 32-bit event register)
uint32_t wfp(void){
    return 0b0011;
}



int main() {
    
    printf("Hello, RISC-V!\n");

    
    // Initialize event handlers before entering the loop
    init_event_handlers();
    register_event_handler(0b0011, handle_event0);

    // Infinite loop
    while(1){
        uint32_t bitmap = wfp();
        process_events(bitmap);
    }


    while (1);
    return 0;  // This should never be reached
}


