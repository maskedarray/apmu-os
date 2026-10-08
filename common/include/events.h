#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <stdint.h>

#define MAX_EVENTS 10

typedef void (*event_handler_t)(uint32_t fired);

int register_event_handler(uint32_t id, uint32_t bitmask, event_handler_t handler);
int unregister_event_handler(uint32_t id);
void init_event_handlers(void);
// Waits on the OR of all registered bitmasks and calls every handler whose
// bitmask overlaps the fired counters. Never returns.
void process_events(void);

#endif
