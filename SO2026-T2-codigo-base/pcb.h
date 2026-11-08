#ifndef PCB_H
#define PCB_H

#include <stdbool.h>

typedef enum {
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED,
    STATE_TERMINATED
} process_state_t;

typedef struct pcb {
    int pid;
    int priority;
    process_state_t state;
    unsigned long elapsed_time;
    unsigned long cpu_time;
    unsigned long blocked_time;
} pcb_t;

#endif // PCB_H
