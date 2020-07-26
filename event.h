#ifndef __EVENT_H__
#define __EVENT_H__

typedef enum {
    EVENT_TYPE_TEST_1,
    EVENT_TYPE_TEST_2,
    EVENT_TYPE_TEST_3,
    EVENT_TYPE_USER_INPUT,
    EVENT_TYPE_END,
}EVENT_TYPE_E;

typedef struct
{
    EVENT_TYPE_E type;
    void *params;
    int len;
}EVENT;

#define EVENT_NUM (EVENT_TYPE_END)
#define EVENT_TYPE_VALID(type)  (type >=0 && type < EVENT_TYPE_END)

#endif