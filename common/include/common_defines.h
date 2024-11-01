#ifndef __COMMON_DEFINES_H__
#define __COMMON_DEFINES_H__


#define PRINT_START_ADDRESS ((volatile char *)0x10427C00)
#define PRINT_END_ADDRESS   ((volatile char *)0x10427FFF)


#define QUEUE_HEAD_ADDR 0x10427800  // Address of head pointer (producer)
#define QUEUE_TAIL_ADDR 0x10427804  // Address of tail pointer (consumer)
#define QUEUE_START_ADDR 0x10427808  // Start of the queue
#define QUEUE_SIZE 0x3F8  // Define the queue size minus head ADDR and Tail ADDR
// QUEUE Size currently 1KB

#endif