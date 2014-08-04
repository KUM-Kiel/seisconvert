COMPILE = $(CC) -c -Wall -pedantic -O2
LINK = $(CC) -o

TARGETS = kum_segy.o test.o test

all: test

kum_segy.o: kum_segy.c kum_segy.h Makefile
	$(COMPILE) kum_segy.c

test.o: test.c Makefile
	$(COMPILE) test.c

test: test.o kum_segy.o Makefile
	$(LINK) test test.o kum_segy.o

clean:
	rm -rf $(TARGETS)
