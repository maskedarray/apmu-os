#ifndef __LATENCY_BINNING_H__
#define __LATENCY_BINNING_H__

#define LATENCY_BIN_STRUCT_OFFSET  0xC00
#define MAX_BINS 10

struct latency_bin_t {
    unsigned int bin_min[MAX_BINS]; // Min is inclusive
    unsigned int bin_max[MAX_BINS]; // Max is exclusive
    unsigned int bin_value[MAX_BINS];
    unsigned int avg_lat;
};

void print_latency_bins(const struct latency_bin_t *latency_bins);
void latency_binning();
void latency_request_handler();
void latency_binning_init();

#endif