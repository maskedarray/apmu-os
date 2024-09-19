#include "dprintf.h"


volatile char *uart = (volatile char *)0x10000000;


// Implement putchar to send data to UART
int putchar(int ch) {
    *uart = (char)ch;
    return ch;
}

// Minimalistic printf implementation
int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    const char *p;
    for (p = fmt; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'd': {  // Integer
                    int i = va_arg(args, int);
                    char buffer[10];
                    char *b = buffer + sizeof(buffer);
                    *(--b) = '\0';
                    if (i == 0) {
                        *(--b) = '0';
                    } else {
                        while (i) {
                            *(--b) = '0' + (i % 10);
                            i /= 10;
                        }
                    }
                    while (*b) putchar(*b++);
                    break;
                }
                case 'c': {  // Character
                    char c = (char)va_arg(args, int);
                    putchar(c);
                    break;
                }
                case 's': {  // String
                    const char *str = va_arg(args, const char*);
                    while (*str) putchar(*str++);
                    break;
                }
                default:
                    putchar('%');
                    putchar(*p);
                    break;
            }
        } else {
            putchar(*p);
        }
    }

    va_end(args);
    return 0;
}