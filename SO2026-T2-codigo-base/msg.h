#ifndef MSG_H
#define MSG_H

typedef struct {
    int pid;
    int page_id;
    bool is_write;
} msg_t;

#endif // MSG_H
