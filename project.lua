local make = require("make")

local m = make{
  cflags = "-Wall -O3 -std=c99"
}

m:lib("seisconvert", {
  m:c("kumy"), m:c("kumy_file"),
  m:c("seed"), m:c("miniseed_file"),
  m:c("segy"),
  m:c("wav"), m:c("wav_file"),
  m:c("filter"), m:c("fm"),
  m:c("buffer"), m:c("number"), m:c("byte")
})

m:lib("tai", {
  m:c("caldate_fmjd"), m:c("caldate_fmt"), m:c("caldate_mjd"), m:c("caldate_norm"),
  m:c("caldate_scan"), m:c("caldate_ster"), m:c("caltime_fmt"), m:c("caltime_scan"),
  m:c("caltime_tai"), m:c("caltime_utc"), m:c("leapsecs_add"), m:c("leapsecs_init"),
  m:c("leapsecs_read"), m:c("leapsecs_sub"), m:c("tai_add"), m:c("tai_now"),
  m:c("tai_pack"), m:c("tai_sub"), m:c("tai_unpack"), m:c("taia_add"),
  m:c("taia_approx"), m:c("taia_fmtfrac"), m:c("taia_frac"), m:c("taia_half"),
  m:c("taia_less"), m:c("taia_now"), m:c("taia_pack"), m:c("taia_sub"),
  m:c("taia_tai"), m:c("taia_unpack"),
  m:c("tai_gps")
})

m:lib("bcd", {
  m:c("bcd_diff"),
  m:c("bcd_mjd"),
  m:c("bcd_parse"),
  m:c("bcd_valid"),
  m:c("bcd_weekday")
})

m:lib("options", {
  m:c("options")
})

m:lib("cli", {
  m:c("cli")
})

m:program("test", {
  m:c("test")
}, {"seisconvert", "tai", "m"})

m:program("wav_test", {
  m:c("wav_test")
}, {"seisconvert", "m"})

m:program("lowpass", {
  m:c("lowpass")
}, {"seisconvert", "m"})

m:program("highpass", {
  m:c("highpass")
}, {"seisconvert", "m"})

m:program("fm", {
  m:c("fm")
}, {"seisconvert", "m"})

m:program("cos", {
  m:c("cos")
}, {"seisconvert", "m"})

m:program("wav_header", {
  m:c("wav_header")
}, {"seisconvert", "m"})

m:program("seedtest", {
  m:c("seedtest")
}, {"seisconvert", "tai"})

m:program("wav2seed", {
  m:c("wav2seed")
}, {"seisconvert", "tai"})

m:program("kumy2wav", {
  m:c("kumy2wav")
}, {"seisconvert", "tai", "options", "cli"})

m:program("kumy2seed", {
  m:c("kumy2seed")
}, {"seisconvert", "tai", "options", "cli"})

m:program("kumy2raw", {
  m:c("kumy2raw")
}, {"seisconvert", "tai", "options", "cli"})

m:program("sdread", {
  m:c("sdread")
}, {"seisconvert", "tai", "bcd", "options", "cli"})

m:program("sdbackup", {
  m:c("sdbackup")
}, {"seisconvert", "options", "cli"})

m:gen("Makefile")
