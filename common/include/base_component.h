#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

#include <stdint.h>
#include <events.h>

#define MAX_COMPONENTS MAX_EVENTS

typedef void (*request_handler_t)(uint32_t *payload, uint32_t size);
typedef void (*hook_t)(void);

typedef struct {
    uint32_t id;
    uint32_t generation;
    uint32_t bitmask;
    event_handler_t event_handler;
    request_handler_t request_handler;
    hook_t init_hook;
    hook_t exit_hook;
} component_t;

// Installs the base component (id 0, doorbell counter) and the request queues.
void base_component_init(void);

#endif
