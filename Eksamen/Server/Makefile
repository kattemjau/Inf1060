CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o server

run:
	./server haiku.jobs 4444


clean:
	rm -f server

check: compile
	valgrind --leak-check=full ./server job.jobs 4444
