CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

CORRUPT=corrupt.job
ALICE=alice.job
SHORT=tooshort2.job
LINUS=linus.job
PROG=prog.job
NOLENGTH=nolength.job

PORT=7777

All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o server

run:
	./server $(NOLENGTH) $(PORT)

testAll: compile
	./server $(CORRUPT) $(PORT)
	./server $(ALICE) $(PORT)
	./server $(SHORT) $(PORT)
	./server $(LINUS) $(PORT)
	./server $(PROG) $(PORT)
	./server $(NOLENGTH) $(PORT)

clean:
	rm -f server

check: compile
	valgrind --leak-check=full ./server $(LINUS) $(PORT)
