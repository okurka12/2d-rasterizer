
CC = gcc  
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g #-DNDEBUG

main.bin: bmp.o main.c
	$(CC) $(CFLAGS) main.c bmp.o -o $@

bmp.o: bmp.c
	$(CC) $(CFLAGS) -c bmp.c

.PHONY: clean
clean:
	rm *.o *.bin

.PHONY: run
run: main.bin
	./main.bin
