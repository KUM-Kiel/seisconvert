# This file was automatically generated. Do not edit!
COMPILE = $(CC) -c -Wall -pedantic -O3 -std=c99 -Isrc/kumy -Isrc/seed -Isrc/segy -Isrc/wav -Isrc/buffer -Isrc/number -Isrc/tai
LINK = $(CC) -Lbuild -o
MAKELIB = $(AR) rcs

TARGETS = build/objects/kumy/kumy.o build/libkumy.a build/objects/seed/seed.o build/objects/seed/miniseed_file.o build/libseed.a build/objects/segy/segy.o build/libsegy.a build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o build/objects/wav/wav_file.o build/libwav.a build/objects/buffer/buffer.o build/libbuffer.a build/objects/number/number.o build/libnumber.a build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o build/libtai.a build/objects/test.o build/objects/wav_test.o build/objects/lowpass.o build/objects/highpass.o build/objects/fm.o build/objects/kumy2wav.o build/objects/cos.o build/objects/wav_header.o build/objects/seedtest.o build/objects/wav2seed.o

all: build/libkumy.a build/libseed.a build/libsegy.a build/libwav.a build/libbuffer.a build/libnumber.a build/libtai.a build/test build/wav_test build/lowpass build/highpass build/fm build/kumy2wav build/cos build/wav_header build/seedtest build/wav2seed
build/test: build/objects/test.o build/libkumy.a build/libwav.a build/libnumber.a build/libtai.a Makefile
	@echo [LD] build/test
	@mkdir -p build/
	@$(LINK) build/test build/objects/test.o -lkumy -lwav -lnumber -ltai -lm
build/wav_test: build/objects/wav_test.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/wav_test
	@mkdir -p build/
	@$(LINK) build/wav_test build/objects/wav_test.o -lwav -lnumber -lm
build/lowpass: build/objects/lowpass.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/lowpass
	@mkdir -p build/
	@$(LINK) build/lowpass build/objects/lowpass.o -lwav -lnumber -lm
build/highpass: build/objects/highpass.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/highpass
	@mkdir -p build/
	@$(LINK) build/highpass build/objects/highpass.o -lwav -lnumber -lm
build/fm: build/objects/fm.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/fm
	@mkdir -p build/
	@$(LINK) build/fm build/objects/fm.o -lwav -lnumber -lm
build/kumy2wav: build/objects/kumy2wav.o build/libwav.a build/libnumber.a build/libkumy.a Makefile
	@echo [LD] build/kumy2wav
	@mkdir -p build/
	@$(LINK) build/kumy2wav build/objects/kumy2wav.o -lwav -lnumber -lkumy
build/cos: build/objects/cos.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/cos
	@mkdir -p build/
	@$(LINK) build/cos build/objects/cos.o -lwav -lnumber -lm
build/wav_header: build/objects/wav_header.o build/libwav.a build/libnumber.a Makefile
	@echo [LD] build/wav_header
	@mkdir -p build/
	@$(LINK) build/wav_header build/objects/wav_header.o -lwav -lnumber -lm
build/seedtest: build/objects/seedtest.o build/libseed.a build/libwav.a build/libnumber.a build/libtai.a Makefile
	@echo [LD] build/seedtest
	@mkdir -p build/
	@$(LINK) build/seedtest build/objects/seedtest.o -lseed -lwav -lnumber -ltai
build/wav2seed: build/objects/wav2seed.o build/libseed.a build/libwav.a build/libnumber.a build/libtai.a Makefile
	@echo [LD] build/wav2seed
	@mkdir -p build/
	@$(LINK) build/wav2seed build/objects/wav2seed.o -lseed -lwav -lnumber -ltai
build/libkumy.a: build/objects/kumy/kumy.o Makefile
	@echo [AR] build/libkumy.a
	@mkdir -p build/
	@$(MAKELIB) build/libkumy.a build/objects/kumy/kumy.o
build/libseed.a: build/objects/seed/seed.o build/objects/seed/miniseed_file.o Makefile
	@echo [AR] build/libseed.a
	@mkdir -p build/
	@$(MAKELIB) build/libseed.a build/objects/seed/seed.o build/objects/seed/miniseed_file.o
build/libsegy.a: build/objects/segy/segy.o Makefile
	@echo [AR] build/libsegy.a
	@mkdir -p build/
	@$(MAKELIB) build/libsegy.a build/objects/segy/segy.o
build/libwav.a: build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o build/objects/wav/wav_file.o Makefile
	@echo [AR] build/libwav.a
	@mkdir -p build/
	@$(MAKELIB) build/libwav.a build/objects/wav/wav.o build/objects/wav/filter.o build/objects/wav/fm.o build/objects/wav/wav_file.o
build/libbuffer.a: build/objects/buffer/buffer.o Makefile
	@echo [AR] build/libbuffer.a
	@mkdir -p build/
	@$(MAKELIB) build/libbuffer.a build/objects/buffer/buffer.o
build/libnumber.a: build/objects/number/number.o Makefile
	@echo [AR] build/libnumber.a
	@mkdir -p build/
	@$(MAKELIB) build/libnumber.a build/objects/number/number.o
build/libtai.a: build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o Makefile
	@echo [AR] build/libtai.a
	@mkdir -p build/
	@$(MAKELIB) build/libtai.a build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o
build/objects/kumy/kumy.o: src/kumy/kumy.c src/kumy/kumy.h Makefile
	@echo [CC] build/objects/kumy/kumy.o
	@mkdir -p build/objects/kumy/
	@$(COMPILE) -o build/objects/kumy/kumy.o src/kumy/kumy.c
build/objects/seed/seed.o: src/seed/seed.c src/seed/seed.h src/number/number.h src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/seed/seed.o
	@mkdir -p build/objects/seed/
	@$(COMPILE) -o build/objects/seed/seed.o src/seed/seed.c
build/objects/seed/miniseed_file.o: src/seed/miniseed_file.c src/seed/miniseed_file.h src/seed/seed.h src/number/number.h Makefile
	@echo [CC] build/objects/seed/miniseed_file.o
	@mkdir -p build/objects/seed/
	@$(COMPILE) -o build/objects/seed/miniseed_file.o src/seed/miniseed_file.c
build/objects/segy/segy.o: src/segy/segy.c src/segy/segy.h Makefile
	@echo [CC] build/objects/segy/segy.o
	@mkdir -p build/objects/segy/
	@$(COMPILE) -o build/objects/segy/segy.o src/segy/segy.c
build/objects/wav/wav.o: src/wav/wav.c src/wav/wav.h src/number/number.h Makefile
	@echo [CC] build/objects/wav/wav.o
	@mkdir -p build/objects/wav/
	@$(COMPILE) -o build/objects/wav/wav.o src/wav/wav.c
build/objects/wav/filter.o: src/wav/filter.c src/wav/filter.h Makefile
	@echo [CC] build/objects/wav/filter.o
	@mkdir -p build/objects/wav/
	@$(COMPILE) -o build/objects/wav/filter.o src/wav/filter.c
build/objects/wav/fm.o: src/wav/fm.c src/wav/fm.h Makefile
	@echo [CC] build/objects/wav/fm.o
	@mkdir -p build/objects/wav/
	@$(COMPILE) -o build/objects/wav/fm.o src/wav/fm.c
build/objects/wav/wav_file.o: src/wav/wav_file.c src/wav/wav_file.h src/wav/wav.h Makefile
	@echo [CC] build/objects/wav/wav_file.o
	@mkdir -p build/objects/wav/
	@$(COMPILE) -o build/objects/wav/wav_file.o src/wav/wav_file.c
build/objects/buffer/buffer.o: src/buffer/buffer.c src/buffer/buffer.h Makefile
	@echo [CC] build/objects/buffer/buffer.o
	@mkdir -p build/objects/buffer/
	@$(COMPILE) -o build/objects/buffer/buffer.o src/buffer/buffer.c
build/objects/number/number.o: src/number/number.c src/number/number.h Makefile
	@echo [CC] build/objects/number/number.o
	@mkdir -p build/objects/number/
	@$(COMPILE) -o build/objects/number/number.o src/number/number.c
build/objects/tai/caldate_fmjd.o: src/tai/caldate_fmjd.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_fmjd.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_fmjd.o src/tai/caldate_fmjd.c
build/objects/tai/caldate_fmt.o: src/tai/caldate_fmt.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_fmt.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_fmt.o src/tai/caldate_fmt.c
build/objects/tai/caldate_mjd.o: src/tai/caldate_mjd.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_mjd.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_mjd.o src/tai/caldate_mjd.c
build/objects/tai/caldate_norm.o: src/tai/caldate_norm.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_norm.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_norm.o src/tai/caldate_norm.c
build/objects/tai/caldate_scan.o: src/tai/caldate_scan.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_scan.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_scan.o src/tai/caldate_scan.c
build/objects/tai/caldate_ster.o: src/tai/caldate_ster.c src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caldate_ster.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caldate_ster.o src/tai/caldate_ster.c
build/objects/tai/caltime_fmt.o: src/tai/caltime_fmt.c src/tai/caldate.h src/tai/caltime.h Makefile
	@echo [CC] build/objects/tai/caltime_fmt.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_fmt.o src/tai/caltime_fmt.c
build/objects/tai/caltime_scan.o: src/tai/caltime_scan.c src/tai/caldate.h src/tai/caltime.h Makefile
	@echo [CC] build/objects/tai/caltime_scan.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_scan.o src/tai/caltime_scan.c
build/objects/tai/caltime_tai.o: src/tai/caltime_tai.c src/tai/caldate.h src/tai/caltime.h src/tai/tai.h src/tai/uint64.h src/tai/leapsecs.h Makefile
	@echo [CC] build/objects/tai/caltime_tai.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_tai.o src/tai/caltime_tai.c
build/objects/tai/caltime_utc.o: src/tai/caltime_utc.c src/tai/caldate.h src/tai/caltime.h src/tai/tai.h src/tai/uint64.h src/tai/leapsecs.h Makefile
	@echo [CC] build/objects/tai/caltime_utc.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_utc.o src/tai/caltime_utc.c
build/objects/tai/leapsecs_add.o: src/tai/leapsecs_add.c src/tai/leapsecs.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/leapsecs_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_add.o src/tai/leapsecs_add.c
build/objects/tai/leapsecs_init.o: src/tai/leapsecs_init.c src/tai/leapsecs.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/leapsecs_init.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_init.o src/tai/leapsecs_init.c
build/objects/tai/leapsecs_read.o: src/tai/leapsecs_read.c src/tai/leapsecs.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/leapsecs_read.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_read.o src/tai/leapsecs_read.c
build/objects/tai/leapsecs_sub.o: src/tai/leapsecs_sub.c src/tai/leapsecs.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/leapsecs_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_sub.o src/tai/leapsecs_sub.c
build/objects/tai/tai_add.o: src/tai/tai_add.c src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/tai_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_add.o src/tai/tai_add.c
build/objects/tai/tai_now.o: src/tai/tai_now.c src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/tai_now.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_now.o src/tai/tai_now.c
build/objects/tai/tai_pack.o: src/tai/tai_pack.c src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/tai_pack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_pack.o src/tai/tai_pack.c
build/objects/tai/tai_sub.o: src/tai/tai_sub.c src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/tai_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_sub.o src/tai/tai_sub.c
build/objects/tai/tai_unpack.o: src/tai/tai_unpack.c src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/tai_unpack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_unpack.o src/tai/tai_unpack.c
build/objects/tai/taia_add.o: src/tai/taia_add.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_add.o src/tai/taia_add.c
build/objects/tai/taia_approx.o: src/tai/taia_approx.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_approx.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_approx.o src/tai/taia_approx.c
build/objects/tai/taia_fmtfrac.o: src/tai/taia_fmtfrac.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_fmtfrac.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_fmtfrac.o src/tai/taia_fmtfrac.c
build/objects/tai/taia_frac.o: src/tai/taia_frac.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_frac.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_frac.o src/tai/taia_frac.c
build/objects/tai/taia_half.o: src/tai/taia_half.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_half.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_half.o src/tai/taia_half.c
build/objects/tai/taia_less.o: src/tai/taia_less.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_less.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_less.o src/tai/taia_less.c
build/objects/tai/taia_now.o: src/tai/taia_now.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_now.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_now.o src/tai/taia_now.c
build/objects/tai/taia_pack.o: src/tai/taia_pack.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_pack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_pack.o src/tai/taia_pack.c
build/objects/tai/taia_sub.o: src/tai/taia_sub.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_sub.o src/tai/taia_sub.c
build/objects/tai/taia_tai.o: src/tai/taia_tai.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_tai.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_tai.o src/tai/taia_tai.c
build/objects/tai/taia_unpack.o: src/tai/taia_unpack.c src/tai/taia.h src/tai/tai.h src/tai/uint64.h Makefile
	@echo [CC] build/objects/tai/taia_unpack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_unpack.o src/tai/taia_unpack.c
build/objects/test.o: src/test.c src/kumy/kumy.h src/wav/wav.h src/wav/fm.h Makefile
	@echo [CC] build/objects/test.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/test.o src/test.c
build/objects/wav_test.o: src/wav_test.c src/wav/wav.h Makefile
	@echo [CC] build/objects/wav_test.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav_test.o src/wav_test.c
build/objects/lowpass.o: src/lowpass.c src/wav/wav.h src/wav/filter.h Makefile
	@echo [CC] build/objects/lowpass.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/lowpass.o src/lowpass.c
build/objects/highpass.o: src/highpass.c src/wav/wav.h src/wav/filter.h Makefile
	@echo [CC] build/objects/highpass.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/highpass.o src/highpass.c
build/objects/fm.o: src/fm.c src/wav/wav.h src/wav/fm.h Makefile
	@echo [CC] build/objects/fm.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/fm.o src/fm.c
build/objects/kumy2wav.o: src/kumy2wav.c src/wav/wav.h src/kumy/kumy.h Makefile
	@echo [CC] build/objects/kumy2wav.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/kumy2wav.o src/kumy2wav.c
build/objects/cos.o: src/cos.c src/wav/wav_file.h src/wav/wav.h Makefile
	@echo [CC] build/objects/cos.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/cos.o src/cos.c
build/objects/wav_header.o: src/wav_header.c src/wav/wav_file.h src/wav/wav.h Makefile
	@echo [CC] build/objects/wav_header.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav_header.o src/wav_header.c
build/objects/seedtest.o: src/seedtest.c src/seed/seed.h Makefile
	@echo [CC] build/objects/seedtest.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/seedtest.o src/seedtest.c
build/objects/wav2seed.o: src/wav2seed.c src/seed/seed.h Makefile
	@echo [CC] build/objects/wav2seed.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav2seed.o src/wav2seed.c
clean:
	rm -rf build
