#include <latency_binning.h>
#include <pmu_hw_desc.h>
#include <common_defines.h>
#include <debug_printf.h>

volatile struct latency_bin_t *latency_bins;

void latency_binning_init(){
    latency_bins = (volatile struct latency_bin_t *)(DSPM_BASE_ADDR + LATENCY_BIN_STRUCT_OFFSET);
    for (int i=0; i< MAX_BINS; i++){
        latency_bins->bin_value[i] = 0;
    }
    print_latency_bins(latency_bins);
    // C0: Max latency recorded since last reset
    unsigned int counter_idx = 0;
    // Reset the counter
    counter_write(counter_idx, 0);
}

void latency_binning(){
    // C0: Max latency recorded since last reset
    unsigned int counter_idx = 0;
    unsigned int current_max_lat = 0;

    // while(1){  
    counter_read(current_max_lat, counter_idx);
    current_max_lat = current_max_lat & 0x7FFFFFFF;

    // latency_bins->avg_lat = current_max_lat; 

    // Increment the corresponding bin
    for (int i = 0; i < MAX_BINS; i++) {
        if (current_max_lat >= latency_bins->bin_min[i] && current_max_lat < latency_bins->bin_max[i]) {
            latency_bins->bin_value[i]++;
            break; 
        }
    }

    // Reset the counter
    counter_write(counter_idx, 0);
    // }
}


void print_latency_bins(const struct latency_bin_t *latency_bins) {
    printf("bin_min: ");
    for (int i = 0; i < MAX_BINS; i++) {
        printf("%d", latency_bins->bin_min[i]);
        if (i < MAX_BINS - 1) {
            printf(", ");
        }
    }
    printf("\n");

    printf("bin_max: ");
    for (int i = 0; i < MAX_BINS; i++) {
        printf("%d", latency_bins->bin_max[i]);
        if (i < MAX_BINS - 1) {
            printf(", ");
        }
    }
    printf("\n");

    printf("bin_value: ");
    for (int i = 0; i < MAX_BINS; i++) {
        printf("%d", latency_bins->bin_value[i]);
        if (i < MAX_BINS - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void latency_request_handler(){
    return;
}