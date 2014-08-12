COMPILE = $(CC) -c -Wall -pedantic -O2 -Isrc/seisconvert
LINK = $(CC) -Lbuild -o
MAKELIB = $(AR) rcs

TARGETS = build/objects/seisconvert/kum_segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/fm.o build/objects/seisconvert/filter.o build/objects/seisconvert/segy.o build/libseisconvert.a build/objects/test.o build/objects/wav_test.o build/objects/lowpass.o build/objects/highpass.o

all: build/libseisconvert.a build/test build/wav_test build/lowpass build/highpass

build/test: build/objects/test.o build/libseisconvert.a Makefile
	@mkdir -p build/
	$(LINK) build/test build/objects/test.o -lseisconvert -lm
build/wav_test: build/objects/wav_test.o build/libseisconvert.a Makefile
	@mkdir -p build/
	$(LINK) build/wav_test build/objects/wav_test.o -lseisconvert -lm
build/lowpass: build/objects/lowpass.o build/libseisconvert.a Makefile
	@mkdir -p build/
	$(LINK) build/lowpass build/objects/lowpass.o -lseisconvert -lm
build/highpass: build/objects/highpass.o build/libseisconvert.a Makefile
	@mkdir -p build/
	$(LINK) build/highpass build/objects/highpass.o -lseisconvert -lm
build/libseisconvert.a: build/objects/seisconvert/kum_segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/fm.o build/objects/seisconvert/filter.o build/objects/seisconvert/segy.o Makefile
	@mkdir -p build/
	$(MAKELIB) build/libseisconvert.a build/objects/seisconvert/kum_segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/fm.o build/objects/seisconvert/filter.o build/objects/seisconvert/segy.o
build/objects/seisconvert/kum_segy.o: src/seisconvert/kum_segy.c src/seisconvert/kum_segy.h Makefile
	@mkdir -p build/objects/seisconvert/
	$(COMPILE) -o build/objects/seisconvert/kum_segy.o src/seisconvert/kum_segy.c
build/objects/seisconvert/wav.o: src/seisconvert/wav.c src/seisconvert/wav.h Makefile
	@mkdir -p build/objects/seisconvert/
	$(COMPILE) -o build/objects/seisconvert/wav.o src/seisconvert/wav.c
build/objects/seisconvert/fm.o: src/seisconvert/fm.c src/seisconvert/fm.h Makefile
	@mkdir -p build/objects/seisconvert/
	$(COMPILE) -o build/objects/seisconvert/fm.o src/seisconvert/fm.c
build/objects/seisconvert/filter.o: src/seisconvert/filter.c src/seisconvert/filter.h Makefile
	@mkdir -p build/objects/seisconvert/
	$(COMPILE) -o build/objects/seisconvert/filter.o src/seisconvert/filter.c
build/objects/seisconvert/segy.o: src/seisconvert/segy.c src/seisconvert/segy.h Makefile
	@mkdir -p build/objects/seisconvert/
	$(COMPILE) -o build/objects/seisconvert/segy.o src/seisconvert/segy.c
build/objects/test.o: src/test.c src/seisconvert/kum_segy.h src/seisconvert/wav.h src/seisconvert/fm.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/test.o src/test.c
build/objects/wav_test.o: src/wav_test.c src/seisconvert/wav.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/wav_test.o src/wav_test.c
build/objects/lowpass.o: src/lowpass.c src/seisconvert/wav.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/lowpass.o src/lowpass.c
build/objects/highpass.o: src/highpass.c src/seisconvert/wav.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/highpass.o src/highpass.c
clean:
	rm -rf build
