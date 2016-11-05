CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g -D_XOPEN_SOURCE

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile run

compile: klient.c
		$(CC) $(CFLAGS) $< -o klient

run:
	./klient 127.0.0.1 4444


clean:
	rm -f klient

check: compile
	valgrind --leak-check=full ./klient 10.0.0.122 4444
