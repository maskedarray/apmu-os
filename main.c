// hello.c
 void main() {
   volatile char *uart = (volatile char *)0x10000000;
   const char *msg = "Hello, RISC-V!\n";
   while (*msg) {
       *uart = *msg++;
   }
   while (1);
 }
