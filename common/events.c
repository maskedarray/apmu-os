#include <events.h>
#include <pmu_hw_desc.h>

typedef struct {
    uint32_t bitmask;
    event_handler_t handler;
} event_obj_t;

static event_obj_t event_list[MAX_EVENTS];
static uint32_t global_event_bitmask;

static void update_global_bitmask(void)
{
    uint32_t i, m = 0;

    for (i = 0; i < MAX_EVENTS; i++)
        m |= event_list[i].bitmask;
    global_event_bitmask = m;
}

int register_event_handler(uint32_t id, uint32_t bitmask, event_handler_t handler)
{
    if (id >= MAX_EVENTS)
        return -1;
    event_list[id].bitmask = handler ? bitmask : 0;
    event_list[id].handler = handler;
    update_global_bitmask();
    return 0;
}

int unregister_event_handler(uint32_t id)
{
    return register_event_handler(id, 0, 0);
}

void init_event_handlers(void)
{
    uint32_t i;

    for (i = 0; i < MAX_EVENTS; i++)
        event_list[i].handler = 0, event_list[i].bitmask = 0;
    global_event_bitmask = 0;
}

void process_events(void)
{
    uint32_t i, fired;

    for (;;) {
        counter_wait_pending(fired, global_event_bitmask);
        for (i = 0; i < MAX_EVENTS; i++)
            if (fired & event_list[i].bitmask)
                event_list[i].handler(fired);
    }
}
