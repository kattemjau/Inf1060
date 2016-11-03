CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile run

compile: server.c
		$(CC) $(CFLAGS) $< -o klient

run:
	./klient 129.0.0.1 293


clean:
	rm -f klient

check: compile
	valgrind --leak-check=full ./klient
