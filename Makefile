CC=gcc
CFLAGS= -Wall -Wextra -std=c99

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile run

compile: leser.c
		$(CC) $^ -o this

run:
	./this


clean:
	rm -f this

check: compile
	valgrind --leak-check=full ./this
