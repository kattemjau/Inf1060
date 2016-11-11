CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE 

All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o server

run:
	./server linus.job 4444


clean:
	rm -f server

check: compile
	valgrind --leak-check=full ./server haiku.job 4444
