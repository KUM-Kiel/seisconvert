COMPILE = $(CC) -c -Wall -pedantic -O2 -Isrc/kumy -Isrc/segy -Isrc/wav
LINK = $(CC) -Lbuild -o
MAKELIB = $(AR) rcs

TARGETS = build/objects/kumy/kumy.o build/libkumy.a build/objects/segy/segy.o build/libsegy.a build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o build/libwav.a build/objects/test.o build/objects/wav_test.o build/objects/lowpass.o build/objects/highpass.o

all: build/libkumy.a build/libsegy.a build/libwav.a build/test build/wav_test build/lowpass build/highpass

build/test: build/objects/test.o build/libkumy.a build/libwav.a Makefile
	@mkdir -p build/
	$(LINK) build/test build/objects/test.o -lkumy -lwav -lm
build/wav_test: build/objects/wav_test.o build/libwav.a Makefile
	@mkdir -p build/
	$(LINK) build/wav_test build/objects/wav_test.o -lwav -lm
build/lowpass: build/objects/lowpass.o build/libwav.a Makefile
	@mkdir -p build/
	$(LINK) build/lowpass build/objects/lowpass.o -lwav -lm
build/highpass: build/objects/highpass.o build/libwav.a Makefile
	@mkdir -p build/
	$(LINK) build/highpass build/objects/highpass.o -lwav -lm
build/libkumy.a: build/objects/kumy/kumy.o Makefile
	@mkdir -p build/
	$(MAKELIB) build/libkumy.a build/objects/kumy/kumy.o
build/libsegy.a: build/objects/segy/segy.o Makefile
	@mkdir -p build/
	$(MAKELIB) build/libsegy.a build/objects/segy/segy.o
build/libwav.a: build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o Makefile
	@mkdir -p build/
	$(MAKELIB) build/libwav.a build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o
build/objects/kumy/kumy.o: src/kumy/kumy.c src/kumy/kumy.h Makefile
	@mkdir -p build/objects/kumy/
	$(COMPILE) -o build/objects/kumy/kumy.o src/kumy/kumy.c
build/objects/segy/segy.o: src/segy/segy.c src/segy/segy.h Makefile
	@mkdir -p build/objects/segy/
	$(COMPILE) -o build/objects/segy/segy.o src/segy/segy.c
build/objects/wav/wav.o: src/wav/wav.c src/wav/wav.h Makefile
	@mkdir -p build/objects/wav/
	$(COMPILE) -o build/objects/wav/wav.o src/wav/wav.c
build/objects/wav/filter.o: src/wav/filter.c src/wav/filter.h Makefile
	@mkdir -p build/objects/wav/
	$(COMPILE) -o build/objects/wav/filter.o src/wav/filter.c
build/objects/wav/fm.o: src/wav/fm.c src/wav/fm.h Makefile
	@mkdir -p build/objects/wav/
	$(COMPILE) -o build/objects/wav/fm.o src/wav/fm.c
build/objects/test.o: src/test.c src/kumy/kumy.h src/wav/wav.h src/wav/fm.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/test.o src/test.c
build/objects/wav_test.o: src/wav_test.c src/wav/wav.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/wav_test.o src/wav_test.c
build/objects/lowpass.o: src/lowpass.c src/wav/wav.h src/wav/filter.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/lowpass.o src/lowpass.c
build/objects/highpass.o: src/highpass.c src/wav/wav.h src/wav/filter.h Makefile
	@mkdir -p build/objects/
	$(COMPILE) -o build/objects/highpass.o src/highpass.c
clean:
	rm -rf build
