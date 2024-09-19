#ifndef __DPRINTF_H__
#define __DPRINTF_H__

#include <stdarg.h>



#define printf dprintf

int dprintf(const char *fmt, ...);


#endif