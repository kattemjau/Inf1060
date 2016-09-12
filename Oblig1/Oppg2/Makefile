CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -gdwarf-4

All: clean compile run

compile:
		$(CC) $(CFLAGS) oppg21.c oppg2b.c -o hello

run:compile
	./hello


clean:
	clear

valgrind:
	valgrind --leak-check=full ./hello
