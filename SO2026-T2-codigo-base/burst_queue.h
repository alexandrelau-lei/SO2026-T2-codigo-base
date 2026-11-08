#ifndef BURST_QUEUE_H
#define BURST_QUEUE_H

#include <stdbool.h>

typedef struct burst_item {
    int type; // 0: CPU, 1: IO
    int duration;
    int page_id;
    bool is_write;
    struct burst_item *next;
} burst_item_t;

typedef struct {
    burst_item_t *head;
    burst_item_t *tail;
} burst_queue_t;

void burst_queue_init(burst_queue_t *q);
void burst_queue_enqueue(burst_queue_t *q, int type, int duration, int page_id, bool is_write);
burst_item_t *burst_queue_dequeue(burst_queue_t *q);
bool burst_queue_is_empty(burst_queue_t *q);

#endif // BURST_QUEUE_H
