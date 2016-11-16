CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

All:compile run

compile: klient.c
		$(CC) $(CFLAGS) $< -o client

run:
	./client tacky.tech 7777


clean:
	rm -f client

check: compile
	valgrind --leak-check=full ./client tacky.tech 7777
