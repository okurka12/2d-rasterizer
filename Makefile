
CC=gcc
AR=ar
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g #-DNDEBUG
LDFLAGS=

.PHONY: all
all: main.bin

# compile main bin 
main.bin: libbmap.so main.c
	$(CC) $(CFLAGS) -o $@ main.c ./libbmap.so 

# put bmp module into a static library
libbmap.a: bmp.o
	$(AR) r $@ $?

# put bmp module into a dynamic library
libbmap.so: bmp.o
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $?

# compile bmp module
bmp.o: bmp.c
	$(CC) $(CFLAGS) -fPIC -c bmp.c

.PHONY: clean
clean:
	rm -f *.o *.bin *.so *.a *.bmp

.PHONY: run
run: main.bin
	./main.bin
