#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct queue_node {
    pcb_t *process;
    struct queue_node *next;
} queue_node_t;

typedef struct {
    queue_node_t *head;
    queue_node_t *tail;
} queue_t;

void queue_init(queue_t *q);
void queue_enqueue(queue_t *q, pcb_t *p);
pcb_t *queue_dequeue(queue_t *q);
bool queue_is_empty(queue_t *q);

#endif // QUEUE_H
