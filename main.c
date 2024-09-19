#include <stddef.h>
#include <stdint.h>
#include <events.h>
#include <debug_printf.h>
void delay(unsigned int count) {
    while (count--) {
        // Simple delay loop
        asm volatile("nop");
    }
}


// Example event handler functions (replace with actual functions)
void handle_event0() {
    // printf("Handler for event 0\n");
    delay(100000000);
    uint64_t mcycle_value = get_cycles(); // Get the 64-bit cycle count

    // Buffer to hold the formatted string
    char buffer[50]; // Ensure it's large enough to hold the string

    // Generate the string
    sprintf(buffer, "Hello, RISC-V! %u\n", (mcycle_value>>32)); // Use %llu for 64-bit unsigned

    // Print the result using printf
    printf("%s", buffer);


        int64_t a = 1234567890123456789LL;
    int64_t b = 9876543210987654321LL;
    int64_t result = a + b;

    sprintf(buffer, "Hello, RISC-V! %lld\n", result); // Use %llu for 64-bit unsigned

    // Print the result using printf
    printf("%s", buffer);
    
    return;
    // Logic for handling event 0
}

// Prototype for the wfp() function (returns 32-bit event register)
uint32_t wfp(void){
    return 0b0011;
}



int main() {
    
    uint32_t mcycle_value;

    // Assembly block to read mcycle CSR
    asm volatile (
        "csrr %0, mcycle"  // Read the mcycle register into mcycle_value
        : "=r" (mcycle_value)  // Output: mcycle_value
    );

    // Print the result using printf
    printf("Hello, RISC-V! %d\n", mcycle_value);

    
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


