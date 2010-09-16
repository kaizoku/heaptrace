CC = gcc
CFLAGS = -g3 -W -Wall
LDFLAGS = -ldl
LIBFLAGS = -shared -Wl,-soname,$@

all: heap_trace.so

heap_trace.o: heap_trace.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<
heap_trace.so: heap_trace.o
	$(CC) $(LDFLAGS) $(LIBFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o

.PHONY: distclean
distclean: clean
	rm -f heap_trace.so
