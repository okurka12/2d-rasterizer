
CC=gcc
AR=ar
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g #-DNDEBUG
LDFLAGS=

BMP_LIB_MODULES= bmp_save.o bmp_image_ctor.o bmp_image_dtor.o

.PHONY: all
all: main.bin

# compile main bin 
main.bin: libbmap.so main.c utils.h
	$(CC) $(CFLAGS) -o $@ main.c ./libbmap.so 

# put bmp module into a static library
libbmap.a: $(BMP_LIB_MODULES)
	$(AR) r $@ $^

# put bmp module into a dynamic library
libbmap.so: $(BMP_LIB_MODULES)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $^

# compile bmp_save
bmp_save.o: bmp_save.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_image_ctor
bmp_image_ctor.o: bmp_image_ctor.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_image_dtor
bmp_image_dtor.o: bmp_image_dtor.c
	$(CC) $(CFLAGS) -fPIC -c $<

.PHONY: clean
clean:
	rm -f *.o *.bin *.so *.a *.bmp

.PHONY: run
run: main.bin
	./main.bin
