// Request-only component: prints the request and replies with each word + 1.

#include <apmu_component.h>

uint32_t component_id;
uint32_t component_bitmask;

void component_request_handler(uint32_t *p, uint32_t size)
{
    uint32_t i;

    printf("hello %d: %d words\n", component_id, size / 4);
    for (i = 0; i < size / 4; i++)
        p[i]++;
    component_reply(p, size / 4);
}
