#ifndef __DPRINTF_H__
#define __DPRINTF_H__

#include <stdarg.h>



#define printf debug_printf

int debug_printf(const char *fmt, ...);


#endif