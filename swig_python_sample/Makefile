SONAME = _LibSample.so
LIB    = libsample

SRCS := $(LIB).c \
       $(LIB)_wrap.c

CFLAGS := $(shell python-config --cflags) -fPIC -Wno-unused-but-set-variable

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

OBJS = $(patsubst %.c,%.o,$(SRCS))

$(SONAME): $(OBJS)
	gcc -shared -Wl,-soname,$(SONAME) -o $(SONAME) $(OBJS)

.PHONY: clean

clean:
	-rm *.so *.o *.d *.pyc $(OBJS) $(SONAME)

swig-generate-extensions:
	swig2.0 -python libsample.h

all: $(SONAME)