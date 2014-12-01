# This file was automatically generated. Do not edit!
COMPILE = $(CC) -c -Wall -O3 -std=c99 -Isrc/seisconvert -Isrc/tai -Isrc/bcd -Isrc/options -Isrc/cli
LINK = $(CC) -Lbuild
MAKELIB = $(AR) rcs

TARGETS = build/libseisconvert.a build/objects/seisconvert/kumy.o build/objects/seisconvert/kumy_file.o build/objects/seisconvert/seed.o build/objects/seisconvert/miniseed_file.o build/objects/seisconvert/segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/wav_file.o build/objects/seisconvert/filter.o build/objects/seisconvert/fm.o build/objects/seisconvert/buffer.o build/objects/seisconvert/number.o build/objects/seisconvert/byte.o build/libtai.a build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o build/objects/tai/tai_gps.o build/libbcd.a build/objects/bcd/bcd_diff.o build/objects/bcd/bcd_mjd.o build/objects/bcd/bcd_parse.o build/objects/bcd/bcd_valid.o build/objects/bcd/bcd_weekday.o build/liboptions.a build/objects/options/options.o build/libcli.a build/objects/cli/cli.o build/test build/objects/test.o build/wav_test build/objects/wav_test.o build/lowpass build/objects/lowpass.o build/highpass build/objects/highpass.o build/fm build/objects/fm.o build/cos build/objects/cos.o build/wav_header build/objects/wav_header.o build/seedtest build/objects/seedtest.o build/wav2seed build/objects/wav2seed.o build/kumy2wav build/objects/kumy2wav.o build/kumy2seed build/objects/kumy2seed.o build/kumy2raw build/objects/kumy2raw.o build/sdread build/objects/sdread.o build/sdbackup build/objects/sdbackup.o

all: build/libseisconvert.a build/libtai.a build/libbcd.a build/liboptions.a build/libcli.a build/test build/wav_test build/lowpass build/highpass build/fm build/cos build/wav_header build/seedtest build/wav2seed build/kumy2wav build/kumy2seed build/kumy2raw build/sdread build/sdbackup
build/test: build/objects/test.o build/libseisconvert.a build/libtai.a Makefile
	@echo [LD] build/test
	@mkdir -p build/
	@$(LINK) -o build/test build/objects/test.o -lseisconvert -ltai -lm
build/wav_test: build/objects/wav_test.o build/libseisconvert.a Makefile
	@echo [LD] build/wav_test
	@mkdir -p build/
	@$(LINK) -o build/wav_test build/objects/wav_test.o -lseisconvert -lm
build/lowpass: build/objects/lowpass.o build/libseisconvert.a Makefile
	@echo [LD] build/lowpass
	@mkdir -p build/
	@$(LINK) -o build/lowpass build/objects/lowpass.o -lseisconvert -lm
build/highpass: build/objects/highpass.o build/libseisconvert.a Makefile
	@echo [LD] build/highpass
	@mkdir -p build/
	@$(LINK) -o build/highpass build/objects/highpass.o -lseisconvert -lm
build/fm: build/objects/fm.o build/libseisconvert.a Makefile
	@echo [LD] build/fm
	@mkdir -p build/
	@$(LINK) -o build/fm build/objects/fm.o -lseisconvert -lm
build/cos: build/objects/cos.o build/libseisconvert.a Makefile
	@echo [LD] build/cos
	@mkdir -p build/
	@$(LINK) -o build/cos build/objects/cos.o -lseisconvert -lm
build/wav_header: build/objects/wav_header.o build/libseisconvert.a Makefile
	@echo [LD] build/wav_header
	@mkdir -p build/
	@$(LINK) -o build/wav_header build/objects/wav_header.o -lseisconvert -lm
build/seedtest: build/objects/seedtest.o build/libseisconvert.a build/libtai.a Makefile
	@echo [LD] build/seedtest
	@mkdir -p build/
	@$(LINK) -o build/seedtest build/objects/seedtest.o -lseisconvert -ltai
build/wav2seed: build/objects/wav2seed.o build/libseisconvert.a build/libtai.a Makefile
	@echo [LD] build/wav2seed
	@mkdir -p build/
	@$(LINK) -o build/wav2seed build/objects/wav2seed.o -lseisconvert -ltai
build/kumy2wav: build/objects/kumy2wav.o build/libseisconvert.a build/libtai.a build/liboptions.a build/libcli.a Makefile
	@echo [LD] build/kumy2wav
	@mkdir -p build/
	@$(LINK) -o build/kumy2wav build/objects/kumy2wav.o -lseisconvert -ltai -loptions -lcli
build/kumy2seed: build/objects/kumy2seed.o build/libseisconvert.a build/libtai.a build/liboptions.a build/libcli.a Makefile
	@echo [LD] build/kumy2seed
	@mkdir -p build/
	@$(LINK) -o build/kumy2seed build/objects/kumy2seed.o -lseisconvert -ltai -loptions -lcli
build/kumy2raw: build/objects/kumy2raw.o build/libseisconvert.a build/libtai.a build/liboptions.a build/libcli.a Makefile
	@echo [LD] build/kumy2raw
	@mkdir -p build/
	@$(LINK) -o build/kumy2raw build/objects/kumy2raw.o -lseisconvert -ltai -loptions -lcli
build/sdread: build/objects/sdread.o build/libseisconvert.a build/libtai.a build/libbcd.a build/liboptions.a build/libcli.a Makefile
	@echo [LD] build/sdread
	@mkdir -p build/
	@$(LINK) -o build/sdread build/objects/sdread.o -lseisconvert -ltai -lbcd -loptions -lcli
build/sdbackup: build/objects/sdbackup.o build/libseisconvert.a build/liboptions.a build/libcli.a Makefile
	@echo [LD] build/sdbackup
	@mkdir -p build/
	@$(LINK) -o build/sdbackup build/objects/sdbackup.o -lseisconvert -loptions -lcli
build/libseisconvert.a: build/objects/seisconvert/kumy.o build/objects/seisconvert/kumy_file.o build/objects/seisconvert/seed.o build/objects/seisconvert/miniseed_file.o build/objects/seisconvert/segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/wav_file.o build/objects/seisconvert/filter.o build/objects/seisconvert/fm.o build/objects/seisconvert/buffer.o build/objects/seisconvert/number.o build/objects/seisconvert/byte.o Makefile
	@echo [AR] build/libseisconvert.a
	@mkdir -p build/
	@$(MAKELIB) build/libseisconvert.a build/objects/seisconvert/kumy.o build/objects/seisconvert/kumy_file.o build/objects/seisconvert/seed.o build/objects/seisconvert/miniseed_file.o build/objects/seisconvert/segy.o build/objects/seisconvert/wav.o build/objects/seisconvert/wav_file.o build/objects/seisconvert/filter.o build/objects/seisconvert/fm.o build/objects/seisconvert/buffer.o build/objects/seisconvert/number.o build/objects/seisconvert/byte.o
build/libtai.a: build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o build/objects/tai/tai_gps.o Makefile
	@echo [AR] build/libtai.a
	@mkdir -p build/
	@$(MAKELIB) build/libtai.a build/objects/tai/caldate_fmjd.o build/objects/tai/caldate_fmt.o build/objects/tai/caldate_mjd.o build/objects/tai/caldate_norm.o build/objects/tai/caldate_scan.o build/objects/tai/caldate_ster.o build/objects/tai/caltime_fmt.o build/objects/tai/caltime_scan.o build/objects/tai/caltime_tai.o build/objects/tai/caltime_utc.o build/objects/tai/leapsecs_add.o build/objects/tai/leapsecs_init.o build/objects/tai/leapsecs_read.o build/objects/tai/leapsecs_sub.o build/objects/tai/tai_add.o build/objects/tai/tai_now.o build/objects/tai/tai_pack.o build/objects/tai/tai_sub.o build/objects/tai/tai_unpack.o build/objects/tai/taia_add.o build/objects/tai/taia_approx.o build/objects/tai/taia_fmtfrac.o build/objects/tai/taia_frac.o build/objects/tai/taia_half.o build/objects/tai/taia_less.o build/objects/tai/taia_now.o build/objects/tai/taia_pack.o build/objects/tai/taia_sub.o build/objects/tai/taia_tai.o build/objects/tai/taia_unpack.o build/objects/tai/tai_gps.o
build/libbcd.a: build/objects/bcd/bcd_diff.o build/objects/bcd/bcd_mjd.o build/objects/bcd/bcd_parse.o build/objects/bcd/bcd_valid.o build/objects/bcd/bcd_weekday.o Makefile
	@echo [AR] build/libbcd.a
	@mkdir -p build/
	@$(MAKELIB) build/libbcd.a build/objects/bcd/bcd_diff.o build/objects/bcd/bcd_mjd.o build/objects/bcd/bcd_parse.o build/objects/bcd/bcd_valid.o build/objects/bcd/bcd_weekday.o
build/liboptions.a: build/objects/options/options.o Makefile
	@echo [AR] build/liboptions.a
	@mkdir -p build/
	@$(MAKELIB) build/liboptions.a build/objects/options/options.o
build/libcli.a: build/objects/cli/cli.o Makefile
	@echo [AR] build/libcli.a
	@mkdir -p build/
	@$(MAKELIB) build/libcli.a build/objects/cli/cli.o
build/objects/seisconvert/kumy.o: src/seisconvert/kumy.c src/seisconvert/kumy.h Makefile
	@echo [CC] build/objects/seisconvert/kumy.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/kumy.o src/seisconvert/kumy.c
build/objects/seisconvert/kumy_file.o: src/seisconvert/kumy_file.c src/seisconvert/kumy_file.h src/seisconvert/kumy.h Makefile
	@echo [CC] build/objects/seisconvert/kumy_file.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/kumy_file.o src/seisconvert/kumy_file.c
build/objects/seisconvert/seed.o: src/seisconvert/seed.c src/seisconvert/seed.h src/tai/taia.h src/tai/tai.h src/seisconvert/number.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/seisconvert/seed.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/seed.o src/seisconvert/seed.c
build/objects/seisconvert/miniseed_file.o: src/seisconvert/miniseed_file.c src/seisconvert/miniseed_file.h src/seisconvert/seed.h src/tai/taia.h src/tai/tai.h src/seisconvert/number.h Makefile
	@echo [CC] build/objects/seisconvert/miniseed_file.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/miniseed_file.o src/seisconvert/miniseed_file.c
build/objects/seisconvert/segy.o: src/seisconvert/segy.c src/seisconvert/segy.h src/seisconvert/number.h Makefile
	@echo [CC] build/objects/seisconvert/segy.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/segy.o src/seisconvert/segy.c
build/objects/seisconvert/wav.o: src/seisconvert/wav.c src/seisconvert/wav.h src/seisconvert/number.h Makefile
	@echo [CC] build/objects/seisconvert/wav.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/wav.o src/seisconvert/wav.c
build/objects/seisconvert/wav_file.o: src/seisconvert/wav_file.c src/seisconvert/wav_file.h src/seisconvert/wav.h Makefile
	@echo [CC] build/objects/seisconvert/wav_file.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/wav_file.o src/seisconvert/wav_file.c
build/objects/seisconvert/filter.o: src/seisconvert/filter.c src/seisconvert/filter.h Makefile
	@echo [CC] build/objects/seisconvert/filter.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/filter.o src/seisconvert/filter.c
build/objects/seisconvert/fm.o: src/seisconvert/fm.c src/seisconvert/fm.h Makefile
	@echo [CC] build/objects/seisconvert/fm.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/fm.o src/seisconvert/fm.c
build/objects/seisconvert/buffer.o: src/seisconvert/buffer.c src/seisconvert/buffer.h Makefile
	@echo [CC] build/objects/seisconvert/buffer.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/buffer.o src/seisconvert/buffer.c
build/objects/seisconvert/number.o: src/seisconvert/number.c src/seisconvert/number.h Makefile
	@echo [CC] build/objects/seisconvert/number.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/number.o src/seisconvert/number.c
build/objects/seisconvert/byte.o: src/seisconvert/byte.c src/seisconvert/byte.h Makefile
	@echo [CC] build/objects/seisconvert/byte.o
	@mkdir -p build/objects/seisconvert/
	@$(COMPILE) -o build/objects/seisconvert/byte.o src/seisconvert/byte.c
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
build/objects/tai/caltime_fmt.o: src/tai/caltime_fmt.c src/tai/caltime.h src/tai/caldate.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/caltime_fmt.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_fmt.o src/tai/caltime_fmt.c
build/objects/tai/caltime_scan.o: src/tai/caltime_scan.c src/tai/caltime.h src/tai/caldate.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/caltime_scan.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_scan.o src/tai/caltime_scan.c
build/objects/tai/caltime_tai.o: src/tai/caltime_tai.c src/tai/tai.h src/tai/leapsecs.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caltime_tai.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_tai.o src/tai/caltime_tai.c
build/objects/tai/caltime_utc.o: src/tai/caltime_utc.c src/tai/tai.h src/tai/leapsecs.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/caltime_utc.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/caltime_utc.o src/tai/caltime_utc.c
build/objects/tai/leapsecs_add.o: src/tai/leapsecs_add.c src/tai/leapsecs.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/leapsecs_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_add.o src/tai/leapsecs_add.c
build/objects/tai/leapsecs_init.o: src/tai/leapsecs_init.c src/tai/leapsecs.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/leapsecs_init.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_init.o src/tai/leapsecs_init.c
build/objects/tai/leapsecs_read.o: src/tai/leapsecs_read.c src/tai/leapsecs.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/leapsecs_read.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_read.o src/tai/leapsecs_read.c
build/objects/tai/leapsecs_sub.o: src/tai/leapsecs_sub.c src/tai/leapsecs.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/leapsecs_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/leapsecs_sub.o src/tai/leapsecs_sub.c
build/objects/tai/tai_add.o: src/tai/tai_add.c src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/tai_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_add.o src/tai/tai_add.c
build/objects/tai/tai_now.o: src/tai/tai_now.c src/tai/tai.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/tai_now.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_now.o src/tai/tai_now.c
build/objects/tai/tai_pack.o: src/tai/tai_pack.c src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/tai_pack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_pack.o src/tai/tai_pack.c
build/objects/tai/tai_sub.o: src/tai/tai_sub.c src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/tai_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_sub.o src/tai/tai_sub.c
build/objects/tai/tai_unpack.o: src/tai/tai_unpack.c src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/tai_unpack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_unpack.o src/tai/tai_unpack.c
build/objects/tai/taia_add.o: src/tai/taia_add.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_add.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_add.o src/tai/taia_add.c
build/objects/tai/taia_approx.o: src/tai/taia_approx.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_approx.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_approx.o src/tai/taia_approx.c
build/objects/tai/taia_fmtfrac.o: src/tai/taia_fmtfrac.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_fmtfrac.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_fmtfrac.o src/tai/taia_fmtfrac.c
build/objects/tai/taia_frac.o: src/tai/taia_frac.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_frac.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_frac.o src/tai/taia_frac.c
build/objects/tai/taia_half.o: src/tai/taia_half.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_half.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_half.o src/tai/taia_half.c
build/objects/tai/taia_less.o: src/tai/taia_less.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_less.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_less.o src/tai/taia_less.c
build/objects/tai/taia_now.o: src/tai/taia_now.c src/tai/taia.h src/tai/tai.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/tai/taia_now.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_now.o src/tai/taia_now.c
build/objects/tai/taia_pack.o: src/tai/taia_pack.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_pack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_pack.o src/tai/taia_pack.c
build/objects/tai/taia_sub.o: src/tai/taia_sub.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_sub.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_sub.o src/tai/taia_sub.c
build/objects/tai/taia_tai.o: src/tai/taia_tai.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_tai.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_tai.o src/tai/taia_tai.c
build/objects/tai/taia_unpack.o: src/tai/taia_unpack.c src/tai/taia.h src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/taia_unpack.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/taia_unpack.o src/tai/taia_unpack.c
build/objects/tai/tai_gps.o: src/tai/tai_gps.c src/tai/tai.h Makefile
	@echo [CC] build/objects/tai/tai_gps.o
	@mkdir -p build/objects/tai/
	@$(COMPILE) -o build/objects/tai/tai_gps.o src/tai/tai_gps.c
build/objects/bcd/bcd_diff.o: src/bcd/bcd_diff.c src/bcd/bcd.h Makefile
	@echo [CC] build/objects/bcd/bcd_diff.o
	@mkdir -p build/objects/bcd/
	@$(COMPILE) -o build/objects/bcd/bcd_diff.o src/bcd/bcd_diff.c
build/objects/bcd/bcd_mjd.o: src/bcd/bcd_mjd.c src/bcd/bcd.h Makefile
	@echo [CC] build/objects/bcd/bcd_mjd.o
	@mkdir -p build/objects/bcd/
	@$(COMPILE) -o build/objects/bcd/bcd_mjd.o src/bcd/bcd_mjd.c
build/objects/bcd/bcd_parse.o: src/bcd/bcd_parse.c src/bcd/bcd.h Makefile
	@echo [CC] build/objects/bcd/bcd_parse.o
	@mkdir -p build/objects/bcd/
	@$(COMPILE) -o build/objects/bcd/bcd_parse.o src/bcd/bcd_parse.c
build/objects/bcd/bcd_valid.o: src/bcd/bcd_valid.c src/bcd/bcd.h Makefile
	@echo [CC] build/objects/bcd/bcd_valid.o
	@mkdir -p build/objects/bcd/
	@$(COMPILE) -o build/objects/bcd/bcd_valid.o src/bcd/bcd_valid.c
build/objects/bcd/bcd_weekday.o: src/bcd/bcd_weekday.c src/bcd/bcd.h Makefile
	@echo [CC] build/objects/bcd/bcd_weekday.o
	@mkdir -p build/objects/bcd/
	@$(COMPILE) -o build/objects/bcd/bcd_weekday.o src/bcd/bcd_weekday.c
build/objects/options/options.o: src/options/options.c src/options/options.h Makefile
	@echo [CC] build/objects/options/options.o
	@mkdir -p build/objects/options/
	@$(COMPILE) -o build/objects/options/options.o src/options/options.c
build/objects/cli/cli.o: src/cli/cli.c src/cli/cli.h Makefile
	@echo [CC] build/objects/cli/cli.o
	@mkdir -p build/objects/cli/
	@$(COMPILE) -o build/objects/cli/cli.o src/cli/cli.c
build/objects/test.o: src/test.c src/seisconvert/kumy.h src/seisconvert/wav.h src/seisconvert/fm.h Makefile
	@echo [CC] build/objects/test.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/test.o src/test.c
build/objects/wav_test.o: src/wav_test.c src/seisconvert/wav.h Makefile
	@echo [CC] build/objects/wav_test.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav_test.o src/wav_test.c
build/objects/lowpass.o: src/lowpass.c src/seisconvert/wav.h src/seisconvert/filter.h Makefile
	@echo [CC] build/objects/lowpass.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/lowpass.o src/lowpass.c
build/objects/highpass.o: src/highpass.c src/seisconvert/wav.h src/seisconvert/filter.h Makefile
	@echo [CC] build/objects/highpass.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/highpass.o src/highpass.c
build/objects/fm.o: src/fm.c src/seisconvert/wav.h src/seisconvert/fm.h Makefile
	@echo [CC] build/objects/fm.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/fm.o src/fm.c
build/objects/cos.o: src/cos.c src/seisconvert/wav_file.h src/seisconvert/wav.h Makefile
	@echo [CC] build/objects/cos.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/cos.o src/cos.c
build/objects/wav_header.o: src/wav_header.c src/seisconvert/wav.h Makefile
	@echo [CC] build/objects/wav_header.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav_header.o src/wav_header.c
build/objects/seedtest.o: src/seedtest.c src/seisconvert/miniseed_file.h src/seisconvert/seed.h src/tai/taia.h src/tai/tai.h src/seisconvert/wav_file.h src/seisconvert/wav.h Makefile
	@echo [CC] build/objects/seedtest.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/seedtest.o src/seedtest.c
build/objects/wav2seed.o: src/wav2seed.c src/seisconvert/miniseed_file.h src/seisconvert/seed.h src/tai/taia.h src/tai/tai.h src/seisconvert/wav_file.h src/seisconvert/wav.h src/tai/caltime.h src/tai/caldate.h Makefile
	@echo [CC] build/objects/wav2seed.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/wav2seed.o src/wav2seed.c
build/objects/kumy2wav.o: src/kumy2wav.c src/seisconvert/kumy_file.h src/seisconvert/kumy.h src/seisconvert/wav_file.h src/seisconvert/wav.h src/tai/taia.h src/tai/tai.h src/tai/caltime.h src/tai/caldate.h src/seisconvert/number.h src/seisconvert/byte.h src/cli/cli.h Makefile
	@echo [CC] build/objects/kumy2wav.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/kumy2wav.o src/kumy2wav.c
build/objects/kumy2seed.o: src/kumy2seed.c src/seisconvert/kumy_file.h src/seisconvert/kumy.h src/seisconvert/miniseed_file.h src/seisconvert/seed.h src/tai/taia.h src/tai/tai.h src/tai/caltime.h src/tai/caldate.h src/seisconvert/number.h src/seisconvert/byte.h src/options/options.h src/cli/cli.h Makefile
	@echo [CC] build/objects/kumy2seed.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/kumy2seed.o src/kumy2seed.c
build/objects/kumy2raw.o: src/kumy2raw.c src/seisconvert/kumy_file.h src/seisconvert/kumy.h src/tai/taia.h src/tai/tai.h src/tai/caltime.h src/tai/caldate.h src/seisconvert/number.h src/seisconvert/byte.h src/options/options.h src/cli/cli.h Makefile
	@echo [CC] build/objects/kumy2raw.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/kumy2raw.o src/kumy2raw.c
build/objects/sdread.o: src/sdread.c src/seisconvert/kumy_file.h src/seisconvert/kumy.h src/seisconvert/number.h src/tai/taia.h src/tai/tai.h src/tai/caltime.h src/tai/caldate.h src/seisconvert/byte.h src/bcd/bcd.h src/options/options.h src/cli/cli.h Makefile
	@echo [CC] build/objects/sdread.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/sdread.o src/sdread.c
build/objects/sdbackup.o: src/sdbackup.c src/seisconvert/number.h src/options/options.h src/cli/cli.h Makefile
	@echo [CC] build/objects/sdbackup.o
	@mkdir -p build/objects/
	@$(COMPILE) -o build/objects/sdbackup.o src/sdbackup.c
clean:
	rm -rf build
