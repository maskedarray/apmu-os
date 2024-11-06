#ifndef __LATENCY_REGULATION_H__
#define __LATENCY_REGULATION_H__

struct lat_reg_t {
    unsigned int target_average_latency;
    unsigned int cnt_n_read;
    unsigned int current_lat;
    unsigned int target_lat;
    unsigned int timer_l;
    unsigned int halt_status;
    unsigned int res_decision;
    unsigned int hal_decision;
    unsigned int resume_resume_time;
    unsigned int resume_halt_status;
    unsigned int resume_timer_l;
    unsigned int resume_current_lat;
    unsigned int resume_target_lat;
    unsigned int halt_halt_time;
    unsigned int halt_halt_status;
    unsigned int halt_timer_l;
    unsigned int halt_current_lat;
    unsigned int halt_target_lat;
};

void print_lat_reg(const volatile struct lat_reg_t *reg);
void latency_regulation();

#endif