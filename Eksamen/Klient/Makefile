CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

All:compile run

compile: klient.c
		$(CC) $(CFLAGS) $< -o client

run:
	./client localhost 7777


clean:
	rm -f client

check: compile
	valgrind --leak-check=full ./client 127.0.0.1 7777
