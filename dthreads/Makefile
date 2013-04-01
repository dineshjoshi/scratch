EXETARGET = dthread

SRCS := dthread.c \

CFLAGS := -I/usr/include -std=gnu99 -O0

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

OBJS = $(patsubst %.c,%.o,$(SRCS))

$(EXETARGET): $(OBJS)
	gcc -o $(EXETARGET) $(OBJS) $(CFLAGS)

.PHONY: clean

clean:
	-rm *.so *.o *.d *.pyc $(OBJS) $(EXETARGET)

all: $(EXETARGET)