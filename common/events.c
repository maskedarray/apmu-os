#include <events.h>

// Global event map
event_map_t event_map[MAX_EVENTS];
size_t event_count = 0;  // Keeps track of the number of registered events

// Function to register an event handler
int register_event_handler(uint32_t bitmask, event_handler_t handler) {
    if (event_count < MAX_EVENTS) {
        event_map[event_count].bitmask = bitmask;
        event_map[event_count].handler = handler;
        event_count++;
        return 0;  // Success
    } else {
        return -1; // Failure (event map is full)
    }
}





// Function to check and call handlers
void process_events(uint32_t bitmap) {
    for (size_t i = 0; i < event_count; i++) {
        if (bitmap & event_map[i].bitmask) {    // Check rd agains bitmask of evey event added in event map
        // This will call the handler even if any of the bits match
        // We can modify this to if ((bitmap & event_map[i].bitmask) == event_map[i].bitmask) 
        // to only call handler when all bits of bitmap and bitmask match
            event_map[i].handler(bitmap);  // Call the handler
        }
    }
}

// Initialize event handlers 
void init_event_handlers() {
    // Add some default events here
    for (int i = 0; i < MAX_EVENTS; i++) {
        event_map[i].handler = 0;  // Set all handlers to null initially 
        event_map[i].bitmask = 0;  // Set bitmask to zero initially
    }
}


