#include <base_component.h>
#include <queue_lib.h>
#include <apmu_abi.h>
#include <debug_printf.h>

#define DSPM_WORD(off) (*(volatile uint32_t *)(uintptr_t)(ABI_DSPM_PHYS + (off)))

extern char _dyn_ispm_start[];

// Base-image functions that dynamic components may call (resolved by the host).
static const abi_export_t exports[] = {
    { "debug_printf",           (uint32_t)debug_printf },
    { "queue_push_get_buffer",  (uint32_t)queue_push_get_buffer },
    { "queue_push_buffer",      (uint32_t)queue_push_buffer },
};

static component_t components[MAX_COMPONENTS];

static int valid_entry(uint32_t addr)
{
    return !addr || (!(addr & 3) && addr >= ABI_ISPM_PHYS &&
                     addr < ABI_ISPM_PHYS + ABI_ISPM_SIZE);
}

static int component_install(const component_t *c)
{
    if (c->id == ABI_BASE_ID || c->id >= MAX_COMPONENTS || !c->generation)
        return ABI_ERR_INVAL;
    if (components[c->id].generation)
        return ABI_ERR_BUSY;
    if ((c->bitmask && !c->event_handler) ||
        !valid_entry((uint32_t)(uintptr_t)c->event_handler) ||
        !valid_entry((uint32_t)(uintptr_t)c->request_handler) ||
        !valid_entry((uint32_t)(uintptr_t)c->init_hook) ||
        !valid_entry((uint32_t)(uintptr_t)c->exit_hook))
        return ABI_ERR_INVAL;

    /* Retire any prefetched instruction before entering newly written ISPM. */
    asm volatile (".word 0x0000100f" ::: "memory"); /* fence.i */
    components[c->id] = *c;
    if (c->init_hook)
        c->init_hook();
    register_event_handler(c->id, c->bitmask, c->event_handler);
    return ABI_OK;
}

static int component_uninstall(uint32_t id, uint32_t generation)
{
    if (id == ABI_BASE_ID || id >= MAX_COMPONENTS)
        return ABI_ERR_INVAL;
    if (!components[id].generation || components[id].generation != generation)
        return ABI_ERR_NOENT;
    unregister_event_handler(id);
    if (components[id].exit_hook)
        components[id].exit_hook();
    components[id].generation = 0;
    return ABI_OK;
}

static void __attribute__((noinline)) respond(uint32_t op, uint32_t status, uint32_t id)
{
    queue_obj_t *r = queue_push_get_buffer(ABI_QUEUE_RSP, 12);

    if (!r)
        return;
    r->req_id = ABI_BASE_ID;
    r->payload[0] = op;
    r->payload[1] = status;
    r->payload[2] = id;
    queue_push_buffer(ABI_QUEUE_RSP, r);
}

static void base_request(uint32_t *p, uint32_t size)
{
    component_t c;
    uint32_t status = ABI_ERR_INVAL, id = 0;

    if (size >= 32 && p[0] == ABI_OP_INSTALL) {
        c.id = id = p[1];
        c.generation = p[2];
        c.bitmask = p[3];
        c.event_handler = (event_handler_t)p[4];
        c.request_handler = (request_handler_t)p[5];
        c.init_hook = (hook_t)p[6];
        c.exit_hook = (hook_t)p[7];
        status = component_install(&c);
        printf("install %d: %d\n", id, status);
    } else if (size >= 12 && p[0] == ABI_OP_UNINSTALL) {
        id = p[1];
        status = component_uninstall(id, p[2]);
        printf("uninstall %d: %d\n", id, status);
    }
    respond(size >= 4 ? p[0] : 0, status, id);
}

// Doorbell: drain the request queue, routing each request by id.
static void base_event(uint32_t fired)
{
    queue_obj_t *req;
    uint32_t size, id;

    (void)fired;
    while ((req = queue_pop_get_buffer(ABI_QUEUE_REQ, &size))) {
        id = req->req_id;
        if (id == ABI_BASE_ID)
            base_request(req->payload, size);
        else if (id < MAX_COMPONENTS && components[id].generation &&
                 components[id].request_handler)
            components[id].request_handler(req->payload, size);
        queue_pop(ABI_QUEUE_REQ);
    }
}

void base_component_init(void)
{
    component_t base = {
        ABI_BASE_ID, 1, 1u << ABI_DOORBELL_COUNTER, base_event, 0, 0, 0
    };

    DSPM_WORD(ABI_STATUS_OFF) = 0;
    DSPM_WORD(ABI_SESSION_OFF) = 0;
    DSPM_WORD(ABI_EXPORTS_OFF) = (uint32_t)exports;
    DSPM_WORD(ABI_NEXPORTS_OFF) = sizeof(exports) / sizeof(exports[0]);
    DSPM_WORD(ABI_DYN_ISPM_OFF) = (uint32_t)_dyn_ispm_start - ABI_ISPM_PHYS;
    queue_init(ABI_QUEUE_REQ);
    queue_init(ABI_QUEUE_RSP);
    init_event_handlers();
    components[ABI_BASE_ID] = base;
    register_event_handler(ABI_BASE_ID, base.bitmask, base.event_handler);
    DSPM_WORD(ABI_STATUS_OFF) = ABI_STATUS_READY;
}
