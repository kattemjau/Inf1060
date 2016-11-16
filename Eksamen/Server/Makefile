CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o server

run:
	./server haiku.job 7777


clean:
	rm -f server

check: compile
	valgrind --leak-check=full ./server haiku.job 7777
