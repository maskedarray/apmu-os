void delay(unsigned int count) {
    while (count--) {
        // Simple delay loop
        asm volatile("nop");
    }
}

void main() {
    volatile char *mem1 = (volatile char *)0x82000000;
    volatile char *mem2 = (volatile char *)0x83000000;

    delay(100000000);

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
