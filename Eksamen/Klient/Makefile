CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

PORT=7777
HOST=localhost


All:compile run

compile: klient.c
		$(CC) $(CFLAGS) $< -o client

run:
	./client $(HOST) $(PORT)

clean:
	rm -f server

check: compile
	valgrind --leak-check=full ./client $(HOST) $(PORT)
