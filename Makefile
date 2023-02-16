
CC = gcc  
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g #-DNDEBUG

main: bmp.o main.c
	$(CC) $(CFLAGS) main.c bmp.o -o main

bmp.o: bmp.c
	$(CC) $(CFLAGS) -c bmp.c

clean:
	rm *.o main