#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "virtmem.h"
#include "scheduler.h"

#define DEFAULT_NUM_FRAMES 3

void print_usage(const char *prog_name) {
    printf("Uso: %s [--vm FIFO|RANDOM|LRU|NRU|CLOCK] [--sched FIFO|RR] [--frames N] <ficheiro_cenario.csv>\n", prog_name);
}

int main(int argc, char *argv[]) {
    vm_algorithm_t vm_algo = VM_FIFO;
    char *vm_algo_name = "FIFO";
    sched_algorithm_t sched_algo = SCHED_FIFO;
    char *sched_algo_name = "FIFO";
    int num_frames = DEFAULT_NUM_FRAMES;
    char *input_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--vm") == 0 && i + 1 < argc) {
            vm_algo_name = argv[i+1];
            if (strcmp(vm_algo_name, "FIFO") == 0) vm_algo = VM_FIFO;
            else if (strcmp(vm_algo_name, "RANDOM") == 0) vm_algo = VM_RANDOM;
            else if (strcmp(vm_algo_name, "LRU") == 0) vm_algo = VM_LRU;
            else if (strcmp(vm_algo_name, "NRU") == 0) vm_algo = VM_NRU;
            else if (strcmp(vm_algo_name, "CLOCK") == 0) vm_algo = VM_CLOCK;
            else {
                fprintf(stderr, "Algoritmo VM desconhecido: %s\n", vm_algo_name);
                return 1;
            }
            i++;
        } else if (strcmp(argv[i], "--sched") == 0 && i + 1 < argc) {
            sched_algo_name = argv[i+1];
            if (strcmp(sched_algo_name, "FIFO") == 0) sched_algo = SCHED_FIFO;
            else if (strcmp(sched_algo_name, "RR") == 0) sched_algo = SCHED_RR;
            else {
                fprintf(stderr, "Algoritmo de Escalonamento desconhecido: %s\n", sched_algo_name);
                return 1;
            }
            i++;
        } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            num_frames = atoi(argv[i+1]);
            i++;
        } else {
            input_file = argv[i];
        }
    }

    if (!input_file) {
        print_usage(argv[0]);
        return 1;
    }

    FILE *f = fopen(input_file, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro de cenario");
        return 1;
    }

    frame_t frames[num_frames];
    for (int i = 0; i < num_frames; i++) {
        frames[i].frame_id = i;
        frames[i].page_id = -1;
        frames[i].process_id = -1;
        frames[i].valid = false;
        frames[i].referenced = false;
        frames[i].dirty = false;
        frames[i].last_accessed = 0;
    }

    vm_init(num_frames, vm_algo);

    printf("====================================================================\n");
    printf(" Simulador SO | Escalonamento: %s | Substituição VM: %s | Molduras: %d\n", sched_algo_name, vm_algo_name, num_frames);
    printf("====================================================================\n\n");

    char line[256];
    unsigned long timer = 1;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        int pid, page;
        char op_type = 'R';

        if (sscanf(line, "%d,%d,%c", &pid, &page, &op_type) >= 2) {
            bool is_write = (op_type == 'W' || op_type == 'w');
            int frame_found = -1;

            for (int i = 0; i < num_frames; i++) {
                if (frames[i].valid && frames[i].page_id == page && frames[i].process_id == pid) {
                    frame_found = i;
                    break;
                }
            }

            if (frame_found != -1) {
                printf("[HIT ] Processo %d -> Pagina Virtual %d no Frame %d (%s)\n\n", pid, page, frame_found, is_write ? "Escrita" : "Leitura");
                vm_access_page(frames, frame_found, is_write, timer);
            } else {
                int victim = vm_select_victim_frame(frames, num_frames, vm_algo);
                printf("[FAULT] Processo %d -> Pagina Virtual %d. Substitui Frame %d (Pagina Anterior %d)\n\n",
                       pid, page, victim, frames[victim].page_id);
                
                frames[victim].valid = true;
                frames[victim].page_id = page;
                frames[victim].process_id = pid;
                frames[victim].referenced = true;
                frames[victim].dirty = is_write;
                frames[victim].last_accessed = timer;
            }
            timer++;
        }
    }

    fclose(f);
    vm_print_stats();

    return 0;
}
