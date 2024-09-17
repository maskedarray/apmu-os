void main()
{
    volatile char *mem1 = (volatile char *)0x82000000;
    volatile char *mem2 = (volatile char *)0x83000000;

    *mem1 = 'A';
    *mem2 = 'B';

    volatile char *uart = (volatile char *)0x10000000;
    const char *msg = "Hello, RISC-V!\n";
    while (*msg)
    {
        *uart = *msg++;
    }
    while (1)
        ;
}
