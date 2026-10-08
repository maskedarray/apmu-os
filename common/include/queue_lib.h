#ifndef QUEUE_LIB_H
#define QUEUE_LIB_H

#include <stdint.h>
#include <apmu_abi.h>

// Single-producer single-consumer queue in DSPM (layout in apmu_abi.h).
// Push and pop are two-phase: get a buffer, fill or read it, then commit.

typedef struct {
    uint32_t size;
    uint32_t req_id;
    uint32_t payload[];
} queue_obj_t;

void queue_init(uint32_t queue_id);
queue_obj_t *queue_push_get_buffer(uint32_t queue_id, uint32_t size);
void queue_push_buffer(uint32_t queue_id, queue_obj_t *obj);
queue_obj_t *queue_pop_get_buffer(uint32_t queue_id, uint32_t *size_out);
void queue_pop(uint32_t queue_id);

#endif
