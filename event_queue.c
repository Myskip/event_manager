#include <stdio.h>
#include <stdlib.h>

#include "event_queue.h"

EVENT_QUEUE *event_queue_new()
{
    EVENT_QUEUE *eq = malloc(sizeof(EVENT_QUEUE));
    if(!eq)
        return NULL;

    eq->queue = uqueue_new();
    if(!eq->queue)
    {
        free(eq);
        return NULL;
    }

    return eq;
}

void event_queue_destroy(EVENT_QUEUE *eq)
{
    if(!eq)
        return;

    uqueue_destroy(eq->queue);
    free(eq);
    eq = NULL;
}

int event_queue_enqueue(EVENT_QUEUE *eq, EVENT *msg)
{
    if(!eq || !msg)
        return UQUEUE_ERROR_INVALID_ARGS;

    return uqueue_enqueue(eq->queue, (void *)msg, sizeof(EVENT));
}

EVENT *event_queue_dequeue(EVENT_QUEUE *eq)
{
    if(!eq)
        return NULL;

    EVENT *msg = NULL;
    int ret = uqueue_dequeue(eq->queue, (void **)&msg, NULL);
    if(ret)
        return NULL;

    return msg;
}