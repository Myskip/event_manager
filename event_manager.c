#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <ulist.h>

#include "event_queue.h"
#include "event_manager.h"

typedef struct
{
    EVENT_TYPE_E type;
    LIST *cb_list;
}EVENT_HANDLER;

typedef struct {
    EVENT_QUEUE *eq;
    EVENT_HANDLER *event_handlers;
    pthread_t pid;
}EVENT_MANAGER;

static void _free_event_handlers(EVENT_HANDLER *handlers)
{
    if(handlers)
        return;

    for(int i=0; i<EVENT_NUM; i++)
    {
        if(handlers[i].cb_list)
            ulist_destroy(handlers[i].cb_list);
    }

    free(handlers);
    handlers = NULL;
}

static EVENT_HANDLER *_malloc_event_handlers()
{
    EVENT_HANDLER *handlers = malloc(EVENT_NUM * sizeof(EVENT_HANDLER));
    memset(handlers, 0, EVENT_NUM * sizeof(EVENT_HANDLER));

    for(int i=0; i<EVENT_NUM; i++)
    {
        LIST *cb_list = ulist_new();
        if(!cb_list)
        {
            printf("new cb_list failed, EVNET:[%d]\n", i);
            goto err;
        }

        handlers[i].cb_list = cb_list;
    }

    return handlers;

err:
    _free_event_handlers(handlers);
    return NULL;
}

static void *event_process(void *p)
{
    EVENT_MANAGER *manager = (EVENT_MANAGER *)p;

    while(TRUE)
    {
        EVENT *event = event_queue_dequeue(manager->eq);
        if(!event)
        {
            usleep(100*1000);
            continue;
        }

        FOR_EACH_NODE(node, manager->event_handlers[event->type].cb_list)
        {
            // node->obj
            EVENT_CB cb = node->obj;
            cb(event->params);
        }
    }
}

EVENT_MANAGER_HANDLE create_event_manager()
{
    EVENT_MANAGER *manager = malloc(sizeof(EVENT_MANAGER));
    if(!manager)
        return NULL;

    manager->eq = event_queue_new();
    if(!manager->eq)
        goto err;

    manager->event_handlers = _malloc_event_handlers();
    if(!manager->event_handlers)
        goto err;

    int ret = pthread_create(&manager->pid, NULL, event_process, (void *)manager);
    if(ret)
    {
        printf("event_process create failed, ret:%d\n", ret);
        goto err;
    }

    return (EVENT_MANAGER_HANDLE)manager;

err:
    if(manager)
    {
        if(manager->eq)
            event_queue_destroy(manager->eq);

        if(manager->event_handlers)
            _free_event_handlers(manager->event_handlers);
    }
    return NULL;
}

int event_manager_reg_event_cb(EVENT_MANAGER_HANDLE em, EVENT_TYPE_E type, EVENT_CB cb)
{
    if(!em || !EVENT_TYPE_VALID(type) || !cb)
        return EVENT_MANAGER_ERROR_INVALID_ARGS;

    EVENT_MANAGER *manager = (EVENT_MANAGER *)em;

    return ulist_add(manager->event_handlers[type].cb_list, cb, sizeof(EVENT_CB));
}

int event_manager_unreg_event_cb(EVENT_MANAGER_HANDLE em, EVENT_TYPE_E type, EVENT_CB cb)
{
    if(!em || !EVENT_TYPE_VALID(type) || !cb)
        return EVENT_MANAGER_ERROR_INVALID_ARGS;

    EVENT_MANAGER *manager = (EVENT_MANAGER *)em;
    return ulist_remove(manager->event_handlers[type].cb_list, cb);
}

int event_manager_on_event_trigger(EVENT_MANAGER_HANDLE em, EVENT *event)
{
    if(!em || !event)
        return EVENT_MANAGER_ERROR_INVALID_ARGS;

    EVENT_MANAGER *manager = (EVENT_MANAGER *)em;

    int ret = event_queue_enqueue(manager->eq, event);
    if(ret)
    {
        printf("event enqueue failed, ret:0x%08x\n", ret);
    }
    return ret;
}