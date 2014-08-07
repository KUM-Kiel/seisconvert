COMPILE = $(CC) -c -Wall -pedantic -O2
LINK = $(CC) -o

TARGETS = kum_segy.o wav.o segy.o fm.o filter.o test.o test wav_test.o wav_test lowpass.o highpass.o highpass

all: test wav_test lowpass highpass segy.o

kum_segy.o: kum_segy.c kum_segy.h Makefile
	$(COMPILE) kum_segy.c

wav.o: wav.c wav.h Makefile
	$(COMPILE) wav.c

segy.o: segy.c segy.h Makefile
	$(COMPILE) segy.c

fm.o: fm.c fm.h Makefile
	$(COMPILE) fm.c

filter.o: filter.c filter.h Makefile
	$(COMPILE) filter.c

test.o: test.c kum_segy.h wav.h fm.h Makefile
	$(COMPILE) test.c

test: test.o kum_segy.o wav.o fm.o Makefile
	$(LINK) test test.o kum_segy.o wav.o fm.o -lm

wav_test.o: wav_test.c wav.h Makefile
	$(COMPILE) wav_test.c

wav_test: wav_test.o wav.o Makefile
	$(LINK) wav_test wav_test.o wav.o -lm

lowpass.o: lowpass.c wav.h filter.h Makefile
	$(COMPILE) lowpass.c

lowpass: lowpass.o wav.o filter.o Makefile
	$(LINK) lowpass lowpass.o wav.o filter.o -lm

highpass.o: highpass.c wav.h filter.h Makefile
	$(COMPILE) highpass.c

highpass: highpass.o wav.o filter.o Makefile
	$(LINK) highpass highpass.o wav.o filter.o -lm

clean:
	rm -rf $(TARGETS)
