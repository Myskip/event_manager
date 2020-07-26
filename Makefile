UQUEUE_INCLUDE=~/codes/application/uqueue
ULIST_INCLUDE=~/codes/application/ulist

SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
LIB:=event_manager.a

.PHONY:all clean lib

all:lib

$(OBJS):
	$(CC) -o $@ -c $(patsubst %.o,%.c,$@) $(CFLAGS) -I $(UQUEUE_INCLUDE) -I $(ULIST_INCLUDE)

lib:$(OBJS)
	$(AR) r $(LIB) $(OBJS)

clean:
	$(RM) r $(OBJS) $(LIB)