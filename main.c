#include <debug_printf.h>
#include <events.h>
#include <base_component.h>

int main(void)
{
    init_print_mem();
    printf("PMU Started!\n");
    base_component_init();
    process_events();
    return 0;
}
