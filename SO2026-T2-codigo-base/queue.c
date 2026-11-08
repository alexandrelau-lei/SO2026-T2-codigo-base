#include "queue.h"
#include <stdlib.h>

void queue_init(queue_t *q) {
    q->head = NULL;
    q->tail = NULL;
}

void queue_enqueue(queue_t *q, pcb_t *p) {
    queue_node_t *node = (queue_node_t *)malloc(sizeof(queue_node_t));
    node->process = p;
    node->next = NULL;

    if (q->tail == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
}

pcb_t *queue_dequeue(queue_t *q) {
    if (q->head == NULL) return NULL;
    queue_node_t *temp = q->head;
    pcb_t *p = temp->process;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    free(temp);
    return p;
}

bool queue_is_empty(queue_t *q) {
    return q->head == NULL;
}
