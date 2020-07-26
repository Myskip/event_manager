#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include <uqueue.h>
#include "event.h"

typedef struct message_queue
{
    UQUEUE *queue;
}EVENT_QUEUE;

EVENT_QUEUE *event_queue_new();
void event_queue_destroy(EVENT_QUEUE *msgq);
int event_queue_enqueue(EVENT_QUEUE *msgq, EVENT *msg);
EVENT *event_queue_dequeue(EVENT_QUEUE *msgq);
#endif