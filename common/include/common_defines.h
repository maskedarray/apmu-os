#ifndef __COMMON_DEFINES_H__
#define __COMMON_DEFINES_H__


#define PRINT_START_ADDRESS ((volatile char *)0x10427C00)
#define PRINT_END_ADDRESS   ((volatile char *)0x10427FFF)


#define QUEUE_HEAD_ADDR 0x82000000  // Address of head pointer (producer)
#define QUEUE_TAIL_ADDR 0x82000004  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR 0x82000008  // Start of the queue
#define QUEUE_SIZE 0x1000  // Define the queue size

#endif