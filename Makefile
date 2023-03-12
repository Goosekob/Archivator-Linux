CC=gcc

CFLAGS=-c -Wall -Wextra -Werror

all: arcivator

arcivator: arcivator.o
	$(CC) arcivator.o -o ./a.out

arcivator.o: arcivator.c
	$(CC) $(CFLAGS) arcivator.c
clean:
	rm -rf arcivator.o