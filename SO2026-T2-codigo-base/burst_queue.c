#include "burst_queue.h"
#include <stdlib.h>

void burst_queue_init(burst_queue_t *q) {
    q->head = NULL;
    q->tail = NULL;
}

void burst_queue_enqueue(burst_queue_t *q, int type, int duration, int page_id, bool is_write) {
    burst_item_t *item = (burst_item_t *)malloc(sizeof(burst_item_t));
    item->type = type;
    item->duration = duration;
    item->page_id = page_id;
    item->is_write = is_write;
    item->next = NULL;

    if (q->tail == NULL) {
        q->head = item;
        q->tail = item;
    } else {
        q->tail->next = item;
        q->tail = item;
    }
}

burst_item_t *burst_queue_dequeue(burst_queue_t *q) {
    if (q->head == NULL) return NULL;
    burst_item_t *temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    return temp;
}

bool burst_queue_is_empty(burst_queue_t *q) {
    return q->head == NULL;
}
