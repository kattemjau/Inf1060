CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE -D _BSD_SOURCE
# brukte -D _BSD_SOURCE pa privat maskin istedet for -D BSD_SOURCE

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
