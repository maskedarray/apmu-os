// Writing a dynamic component: include this, define the fields you need (any
// may be left out), and build with `make components`. The host links the
// object into free ISPM/DSPM and sets component_id at install.
#ifndef __APMU_COMPONENT_H__
#define __APMU_COMPONENT_H__

#include <stdint.h>
#include <apmu_abi.h>
#include <queue_lib.h>
#include <debug_printf.h>

extern uint32_t component_id;
extern uint32_t component_bitmask;      // counters that wake component_event_handler
void component_event_handler(uint32_t fired);
void component_request_handler(uint32_t *payload, uint32_t size);
void init_hook(void);
void exit_hook(void);

// Send n words to the host as a response from this component.
// Returns 0, or -1 if the response queue is full.
static inline int component_reply(const uint32_t *w, uint32_t n)
{
    queue_obj_t *r = queue_push_get_buffer(ABI_QUEUE_RSP, 4 * n);
    uint32_t i;

    if (!r)
        return -1;
    r->req_id = component_id;
    for (i = 0; i < n; i++)
        r->payload[i] = w[i];
    queue_push_buffer(ABI_QUEUE_RSP, r);
    return 0;
}

#endif
