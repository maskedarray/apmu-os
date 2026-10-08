#include <queue_lib.h>

#ifndef QUEUE_DSPM_BASE
#define QUEUE_DSPM_BASE ABI_DSPM_PHYS
#endif

#define QUEUE(id)   ((volatile uint32_t *)(uintptr_t)(QUEUE_DSPM_BASE + \
                     ((id) == ABI_QUEUE_REQ ? ABI_REQ_Q_OFF : ABI_RSP_Q_OFF)))
#define HEAD(id)    QUEUE(id)[0]
#define TAIL(id)    QUEUE(id)[1]
#define OBJ(id, o)  ((queue_obj_t *)((uintptr_t)&QUEUE(id)[2] + (o)))

static uint32_t obj_len(uint32_t size)
{
    return 8 + ((size + 3) & ~3u);
}

void queue_init(uint32_t id)
{
    HEAD(id) = 0;
    TAIL(id) = 0;
}

// The producer never lets head catch up with tail: head == tail means empty.
queue_obj_t *queue_push_get_buffer(uint32_t id, uint32_t size)
{
    uint32_t head = HEAD(id), tail = TAIL(id), n = obj_len(size), pos = head;
    queue_obj_t *obj;

    if (head >= tail) {
        uint32_t end = ABI_Q_DATA_SIZE - head;
        if (!(n < end || (n == end && tail != 0))) {
            if (n >= tail)
                return 0;
            OBJ(id, head)->size = ABI_Q_WRAP;
            pos = 0;
        }
    } else if (n >= tail - head) {
        return 0;
    }
    obj = OBJ(id, pos);
    obj->size = size;
    return obj;
}

void queue_push_buffer(uint32_t id, queue_obj_t *obj)
{
    uint32_t head = (uintptr_t)obj - (uintptr_t)OBJ(id, 0) + obj_len(obj->size);

    HEAD(id) = head == ABI_Q_DATA_SIZE ? 0 : head;
}

queue_obj_t *queue_pop_get_buffer(uint32_t id, uint32_t *size_out)
{
    uint32_t tail = TAIL(id);

    if (tail == HEAD(id))
        return 0;
    if (OBJ(id, tail)->size == ABI_Q_WRAP) {
        TAIL(id) = tail = 0;
        if (tail == HEAD(id))
            return 0;
    }
    *size_out = OBJ(id, tail)->size;
    return OBJ(id, tail);
}

void queue_pop(uint32_t id)
{
    uint32_t tail = TAIL(id) + obj_len(OBJ(id, TAIL(id))->size);

    TAIL(id) = tail == ABI_Q_DATA_SIZE ? 0 : tail;
}
