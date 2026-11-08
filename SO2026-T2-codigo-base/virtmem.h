#ifndef VIRTMEM_H
#define VIRTMEM_H

#include "virtmem_types.h"
#include <stddef.h>

void vm_init(int num_frames, vm_algorithm_t algo);
int vm_select_victim_frame(frame_t frames[], int num_frames, vm_algorithm_t algo);
void vm_access_page(frame_t frames[], int frame_id, bool is_write, unsigned long current_time);
void vm_print_stats(void);

#endif // VIRTMEM_H
