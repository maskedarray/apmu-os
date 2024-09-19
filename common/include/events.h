#ifndef __EVENTS_H__
#define __EVENTS_H__
#include <stddef.h>
#include <stdint.h>

// Define the number of events (assuming 32 events corresponding to 32 bits in rd)
#define NUM_EVENTS 32
#define MAX_EVENTS 10

// Event handler function prototype
typedef void (*event_handler_t)(void);

// Struct to map bitmasks to handlers
typedef struct {
    uint32_t bitmask;
    event_handler_t handler;
} event_map_t;



void init_event_handlers();
void process_events(uint32_t bitmap);
int register_event_handler(uint32_t bitmask, event_handler_t handler);




#endif