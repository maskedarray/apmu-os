#ifndef __DPRINTF_H__
#define __DPRINTF_H__

#include <stdarg.h>


volatile char *uart = (volatile char *)0x10000000;

#define printf dprintf

int dprintf(const char *fmt, ...);


#endif