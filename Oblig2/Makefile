CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -g

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile run

compile: main.c
		$(CC) $(CFLAGS) $< -o hei

run:
	./hei "5router.dat"


clean:
	rm -f hei

check: compile
	valgrind --leak-check=full ./hei "5router.dat"
