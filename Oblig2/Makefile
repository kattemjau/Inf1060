CC=gcc
CFLAGS= -Wall -Wextra -std=c99

#$^ hustak trekker inn filer beskrevet (run: fil1 fil2)
#$< pil venstre tar kun det forste argumentet

# .PHONY: sier hvilke filer som ikke inneholder faktiske filer.

#$@ skriv eks compile:

All:compile

compile: main.c
		$(CC) $< -o hei

run:
	./hei


clean:
	rm -f hei

check: compile
	valgrind --leak-check=full ./hei
