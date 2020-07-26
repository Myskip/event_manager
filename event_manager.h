#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "event.h"

typedef enum {
    EVENT_MANAGER_ERROR_INVALID_ARGS = 0x01010001,
}EVENT_MANAGER_ERROR_E;

typedef void* EVENT_MANAGER_HANDLE;
typedef void (*EVENT_CB)(void *);

EVENT_MANAGER_HANDLE create_event_manager();
int event_manager_reg_event_cb(EVENT_MANAGER_HANDLE em, EVENT_TYPE_E type, EVENT_CB cb);
int event_manager_unreg_event_cb(EVENT_MANAGER_HANDLE em, EVENT_TYPE_E type, EVENT_CB cb);
int event_manager_on_event_trigger(EVENT_MANAGER_HANDLE em, EVENT *event);
#endif