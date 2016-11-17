CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

CORRUPT=corrupt.job
ALICE=alice.job
SHORT=tooshort2.job
LINUS=linus.job
PROG=prog.job


All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o server

run:
	./server $(LINUS) 7777

testAll: compile
	./server $(CORRUPT) 7777
	./server $(ALICE) 7777
	./server $(SHORT) 7777
	./server $(LINUS) 7777
	./server $(PROG) 7777

clean:
	rm -f *.o

check: compile
	valgrind --leak-check=full ./server haiku.job 7777
