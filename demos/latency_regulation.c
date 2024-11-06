#include <latency_regulation.h>
#include <common_defines.h>
#include <pmu_hw_desc.h>
#include <debug_printf.h>

#define LATENCY_REG_STRUCT_OFFSET  0x900
#define DEBUG_HALT      0x200
#define DEBUG_RESUME    0x208

#define HALT            101
#define RESUME          303

#define debug_halt(core_id)     write_32b(DEBUG_HALT, core_id)
#define debug_resume(core_id)   write_32b(DEBUG_RESUME, core_id)

void print_lat_reg(const volatile struct lat_reg_t *reg) {
    printf("printing structure data\n");
    printf("target_average_latency: %d\n", reg->target_average_latency);
    printf("cnt_n_read: %d\n", reg->cnt_n_read);
    printf("current_lat: %d\n", reg->current_lat);
    printf("target_lat: %d\n", reg->target_lat);
    printf("timer_l: %d\n", reg->timer_l);
    printf("halt_status: %d\n", reg->halt_status);
    printf("res_decision: %d\n", reg->res_decision);
    printf("hal_decision: %d\n", reg->hal_decision);
    printf("resume_resume_time: %d\n", reg->resume_resume_time);
    printf("resume_halt_status: %d\n", reg->resume_halt_status);
    printf("resume_timer_l: %d\n", reg->resume_timer_l);
    printf("resume_current_lat: %d\n", reg->resume_current_lat);
    printf("resume_target_lat: %d\n", reg->resume_target_lat);
    printf("halt_halt_time: %d\n", reg->halt_halt_time);
    printf("halt_halt_status: %d\n", reg->halt_halt_status);
    printf("halt_timer_l: %d\n", reg->halt_timer_l);
    printf("halt_current_lat: %d\n", reg->halt_current_lat);
    printf("halt_target_lat: %d\n", reg->halt_target_lat);
}

void latency_regulation() {
    
    volatile struct lat_reg_t *lat_reg = (volatile struct lat_reg_t *)(DSPM_BASE_ADDR + LATENCY_REG_STRUCT_OFFSET);
    // PMU Timer.
    volatile int unsigned timer_l = 0;

    // Read requests and read latencies, from counters.
    volatile int unsigned cnt_n_read,  cnt_read_lat;

    // Used for total latency computation and comparisons.
    volatile int unsigned current_lat = 0;
    volatile int unsigned target_lat  = 0;

    int unsigned zero        = 0;
    int unsigned core_idx    = 0;   // Used to loop over non-CUA cores.
    int unsigned counter_idx = 0;   // Used to loop over PMU counters.

    int unsigned halt_status     = RESUME;
    int unsigned last_timestamp  = 0;
    int unsigned halt_time       = 0;
    int unsigned resume_time     = 0;

    volatile int unsigned res_decision = 0;
    volatile int unsigned hal_decision = 0;

    // This is the target read_latency that the PMU should maintain for CUA.
    volatile int unsigned target_avg_lat = lat_reg->target_average_latency;
    

    // Every halt-resume operation is logged into the DSPM with a timestamp.
    last_timestamp = read_32b(TIMER_ADDR);

    // Initialization
    lat_reg->resume_current_lat = 0;
    lat_reg->resume_halt_status = 0;
    lat_reg->resume_target_lat = 0;
    lat_reg->resume_timer_l = 0;
    lat_reg->halt_current_lat = 0;
    lat_reg->halt_halt_status = 0;
    lat_reg->halt_target_lat = 0;
    lat_reg->halt_timer_l = 0;

    // print_lat_reg(lat_reg);

    while (1) {
        // Read necessary PMU counters.
        // C0
        counter_idx = 0;
        counter_read(cnt_n_read, counter_idx);
        cnt_n_read = cnt_n_read & 0x7FFFFFFF;

        // C1
        counter_idx = 1;
        counter_read(cnt_read_lat, counter_idx);

        current_lat = cnt_read_lat & 0x7FFFFFFF;
        target_lat  = cnt_n_read * target_avg_lat;
       
        lat_reg->cnt_n_read = cnt_n_read;
        lat_reg->current_lat = current_lat;
        lat_reg->target_lat = target_lat;

        timer_l = read_32b(TIMER_ADDR);

        lat_reg->timer_l = timer_l;
        lat_reg->halt_status = halt_status;

        res_decision = (current_lat <= target_lat);
        hal_decision = (current_lat > target_lat);

        lat_reg->res_decision = res_decision;
        lat_reg->hal_decision = hal_decision;
        
        // for (int l=0; l<50; l++) {
        //     write_32b(DSPM_BASE_ADDR+0x1600+l*4, l*l);
        // }

        // HALT
        if (hal_decision && (halt_status == RESUME)) {
            // Halt all cores if not already halted.
            core_idx = 1;
            write_32b(DEBUG_HALT, core_idx);
            // core_idx = 2;
            // write_32b(DEBUG_HALT, core_idx);
            // core_idx = 3;
            // write_32b(DEBUG_HALT, core_idx);
            halt_status = HALT;

            resume_time = resume_time + (timer_l - last_timestamp);

            lat_reg->resume_resume_time = resume_time;
            lat_reg->halt_halt_status = halt_status;
            lat_reg->halt_timer_l = timer_l;
            lat_reg->halt_current_lat = current_lat;
            lat_reg->halt_target_lat = target_lat;

            last_timestamp = timer_l;
        // RESUME
        } else if (res_decision && (halt_status == HALT)) {
            // Resume all cores if halted.
            core_idx = 1;
            write_32b(DEBUG_RESUME, core_idx);
            // core_idx = 2;
            // write_32b(DEBUG_RESUME, core_idx);
            // core_idx = 3;
            // write_32b(DEBUG_RESUME, core_idx);
            halt_status = RESUME;

            halt_time = halt_time + (timer_l - last_timestamp);

            lat_reg->halt_halt_time = halt_time;
            lat_reg->resume_halt_status = halt_status;
            lat_reg->resume_timer_l = timer_l;
            lat_reg->resume_current_lat = current_lat;
            lat_reg->resume_target_lat = target_lat;

            last_timestamp = timer_l;
        }
    }

    return;
}