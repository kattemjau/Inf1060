CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -gdwarf-4 -g

All: clean compile run

compile:
		$(CC) $(CFLAGS) oppg21.c oppg2.c -o hello

run:compile
	./hello


clean:
	clear

check: compile
	valgrind --leak-check=full ./hello
