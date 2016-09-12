CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -gdwarf-4

All: compile run

compile:
		$(CC) $(CFLAGS) oppg3.c -o hello

run:
	./hello


clean:
	rm -r ./hello

valgrind:
	valgrind --leak-check=full ./hello
