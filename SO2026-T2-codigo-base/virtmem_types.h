#ifndef VIRTMEM_TYPES_H
#define VIRTMEM_TYPES_H

#include <stdbool.h>

typedef enum {
    VM_FIFO,
    VM_RANDOM,
    VM_LRU,
    VM_NRU,
    VM_CLOCK
} vm_algorithm_t;

typedef struct frame {
    int frame_id;
    int page_id;
    int process_id;
    bool valid;
    bool referenced;            // Bit R (NRU e CLOCK)
    bool dirty;                 // Bit D (NRU)
    unsigned long last_accessed; // Timestamp (LRU)
} frame_t;

typedef struct page_table_entry {
    int frame_id;
    bool valid;
} page_table_entry_t;

#endif // VIRTMEM_TYPES_H
