COMPILE = $(CC) -c -Wall -pedantic -O2
LINK = $(CC) -o

TARGETS = kum_segy.o wav.o fm.o test.o test wav_test.o wav_test

all: test wav_test

kum_segy.o: kum_segy.c kum_segy.h Makefile
	$(COMPILE) kum_segy.c

wav.o: wav.c wav.h Makefile
	$(COMPILE) wav.c

fm.o: fm.c fm.h Makefile
	$(COMPILE) fm.c

test.o: test.c kum_segy.h wav.h fm.h Makefile
	$(COMPILE) test.c

test: test.o kum_segy.o wav.o fm.o Makefile
	$(LINK) test test.o kum_segy.o wav.o fm.o -lm

wav_test.o: wav_test.c wav.h Makefile
	$(COMPILE) wav_test.c

wav_test: wav_test.o wav.o Makefile
	$(LINK) wav_test wav_test.o wav.o -lm

clean:
	rm -rf $(TARGETS)
