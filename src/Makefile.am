SUBDIRS = bcd cli options seisconvert tai

bin_PROGRAMS = sdread sdbackup kumy2seed kumy2wav kumy2raw

sdread_SOURCES = sdread.c
sdread_CFLAGS = -I$(top_srcdir)/src/seisconvert -I$(top_srcdir)/src/tai -I$(top_srcdir)/src/bcd -I$(top_srcdir)/src/options -I$(top_srcdir)/src/cli
sdread_LDADD = seisconvert/libseisconvert.la tai/libtai.la bcd/libbcd.la options/liboptions.la cli/libcli.la

sdbackup_SOURCES = sdbackup.c
sdbackup_CFLAGS = -I$(top_srcdir)/src/seisconvert -I$(top_srcdir)/src/options -I$(top_srcdir)/src/cli
sdbackup_LDADD = seisconvert/libseisconvert.la options/liboptions.la cli/libcli.la

kumy2seed_SOURCES = kumy2seed.c
kumy2seed_CFLAGS = -I$(top_srcdir)/src/seisconvert -I$(top_srcdir)/src/tai -I$(top_srcdir)/src/options -I$(top_srcdir)/src/cli
kumy2seed_LDADD = seisconvert/libseisconvert.la tai/libtai.la options/liboptions.la cli/libcli.la

kumy2wav_SOURCES = kumy2wav.c
kumy2wav_CFLAGS = -I$(top_srcdir)/src/seisconvert -I$(top_srcdir)/src/tai -I$(top_srcdir)/src/options -I$(top_srcdir)/src/cli
kumy2wav_LDADD = seisconvert/libseisconvert.la tai/libtai.la options/liboptions.la cli/libcli.la

kumy2raw_SOURCES = kumy2raw.c
kumy2raw_CFLAGS = -I$(top_srcdir)/src/seisconvert -I$(top_srcdir)/src/tai -I$(top_srcdir)/src/options -I$(top_srcdir)/src/cli
kumy2raw_LDADD = seisconvert/libseisconvert.la tai/libtai.la options/liboptions.la cli/libcli.la
