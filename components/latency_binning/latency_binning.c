// Histogram of DRAM read latency. Counter 1 keeps the maximum latency seen
// since the last sample; every update wakes the event handler, which bins it.
// Requests: {1} clears the histogram, {2} replies with the 10 bin values,
// {3, core} counts only reads issued by core 0-3 (any other value: all cores),
// {4, lo0..lo9} sets the ascending bin lower bounds and clears.

#include <apmu_component.h>
#include <pmu_hw_desc.h>

#define CNT     1
#define NBINS   10
#define WR32(a, v) (*(volatile uint32_t *)(a) = (v))

uint32_t component_id;
uint32_t component_bitmask = 1u << CNT;

// Bin i holds latencies in [bin_lo[i], bin_lo[i+1]); the last one is open.
static uint32_t bin_lo[NBINS] = { 0, 4, 8, 12, 16, 20, 24, 32, 48, 64 };
static uint32_t bin_value[NBINS];

static void clear(void)
{
    volatile uint32_t *v = bin_value;
    uint32_t i;

    for (i = 0; i < NBINS; i++)
        v[i] = 0;
}

void component_event_handler(uint32_t fired)
{
    uint32_t lat, i;

    (void)fired;
    counter_read(lat, CNT);
    for (i = NBINS; i-- > 0;)
        if (lat >= bin_lo[i]) {
            bin_value[i]++;
            break;
        }
    counter_write(CNT, 0);
}

void component_request_handler(uint32_t *p, uint32_t size)
{
    uint32_t i;

    if (size < 4)
        return;
    if (p[0] == 1)
        clear();
    if (p[0] == 3 && size >= 8)
        WR32(EVENT_SEL_ADDR(CNT), p[1] < 4 ? MEM_RD_RES_CORE(p[1]) : MEM_RD_RES);
    if (p[0] == 4 && size >= 4 + 4 * NBINS) {
        for (i = 0; i < NBINS; i++)
            bin_lo[i] = p[1 + i];
        clear();
    }
    if (p[0] == 2)
        component_reply(bin_value, NBINS);
}

void init_hook(void)
{
    clear();
    WR32(EVENT_INFO_ADDR(CNT), MAX_RESP_LAT);
    WR32(COUNTER_ADDR(CNT), 0);
    WR32(EVENT_SEL_ADDR(CNT), MEM_RD_RES);
}

void exit_hook(void)
{
    WR32(EVENT_SEL_ADDR(CNT), 0);
    WR32(COUNTER_ADDR(CNT), 0);
}
