#include "virtmem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int clock_hand = 0;
static int fifo_hand = 0;
static unsigned long page_faults = 0;
static unsigned long swap_reads = 0;
static unsigned long swap_writes = 0;

void vm_init(int num_frames, vm_algorithm_t algo) {
    (void)num_frames;
    (void)algo;
    clock_hand = 0;
    fifo_hand = 0;
    page_faults = 0;
    swap_reads = 0;
    swap_writes = 0;
}

int vm_select_victim_frame(frame_t frames[], int num_frames, vm_algorithm_t algo) {
    page_faults++;
    int victim = 0;

    switch (algo) {
        case VM_FIFO:
            victim = fifo_hand;
            fifo_hand = (fifo_hand + 1) % num_frames;
            break;

        case VM_RANDOM:
            victim = rand() % num_frames;
            break;

        case VM_LRU: {
            unsigned long min_time = frames[0].last_accessed;
            victim = 0;
            for (int i = 1; i < num_frames; i++) {
                if (frames[i].last_accessed < min_time) {
                    min_time = frames[i].last_accessed;
                    victim = i;
                }
            }
            break;
        }

        case VM_NRU: {
            int class_frames[4] = {-1, -1, -1, -1};
            for (int i = 0; i < num_frames; i++) {
                int r = frames[i].referenced ? 1 : 0;
                int d = frames[i].dirty ? 1 : 0;
                int cat = (r << 1) | d;
                if (class_frames[cat] == -1) {
                    class_frames[cat] = i;
                }
                if (cat == 0) break;
            }
            for (int c = 0; c < 4; c++) {
                if (class_frames[c] != -1) {
                    victim = class_frames[c];
                    printf("  [NRU Detail] Selecionada moldura da Classe %d (R=%d, D=%d)\n\n",
                           c, frames[victim].referenced ? 1 : 0, frames[victim].dirty ? 1 : 0);
                    break;
                }
            }
            break;
        }

        case VM_CLOCK: {
            while (1) {
                if (!frames[clock_hand].referenced) {
                    victim = clock_hand;
                    clock_hand = (clock_hand + 1) % num_frames;
                    break;
                }
                frames[clock_hand].referenced = false;
                clock_hand = (clock_hand + 1) % num_frames;
            }
            break;
        }
    }

    if (frames[victim].valid) {
        if (frames[victim].dirty) {
            swap_writes++;
            printf("  [EVICTION] Moldura %d (Página %d, Processo %d) estava DIRTY (D=1) -> Gerou Swap Write\n\n",
                   victim, frames[victim].page_id, frames[victim].process_id);
        } else {
            printf("  [EVICTION] Moldura %d (Página %d, Processo %d) estava LIMPA (D=0) -> Sem Swap Write\n\n",
                   victim, frames[victim].page_id, frames[victim].process_id);
        }
        swap_reads++;
    } else {
        printf("  [EVICTION] Moldura %d estava vazia (-1) -> Carregamento inicial\n\n", victim);
        swap_reads++;
    }

    return victim;
}

void vm_access_page(frame_t frames[], int frame_id, bool is_write, unsigned long current_time) {
    frames[frame_id].referenced = true;
    frames[frame_id].last_accessed = current_time;
    if (is_write) {
        frames[frame_id].dirty = true;
    }
}

void vm_print_stats(void) {
    printf("\n========================================\n");
    printf("     Estatísticas de Memória Virtual    \n");
    printf("========================================\n");
    printf("Faltas de Página (Page Faults) : %lu\n", page_faults);
    printf("Operações Swap Read (Leituras)  : %lu\n", swap_reads);
    printf("Operações Swap Write (Escritas) : %lu\n", swap_writes);
    printf("Total Operações de Swap (I/O)   : %lu\n", swap_reads + swap_writes);
    printf("========================================\n");
}