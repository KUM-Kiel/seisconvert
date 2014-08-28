local make = require("make")

local m = make{
  cflags = "-Wall -pedantic -O3 -std=c99"
}

m:lib("kumy", {
  m:c("kumy"), m:c("kumy_file")
})

m:lib("seed", {
  m:c("seed"), m:c("miniseed_file")
})

m:lib("segy", {
  m:c("segy")
})

m:lib("wav", {
  m:c("wav"), m:c("filter"), m:c("fm"), m:c("wav_file")
})

m:lib("buffer", {
  m:c("buffer")
})

m:lib("number", {
  m:c("number")
})

m:lib("tai", {
  m:c("caldate_fmjd"), m:c("caldate_fmt"), m:c("caldate_mjd"), m:c("caldate_norm"),
  m:c("caldate_scan"), m:c("caldate_ster"), m:c("caltime_fmt"), m:c("caltime_scan"),
  m:c("caltime_tai"), m:c("caltime_utc"), m:c("leapsecs_add"), m:c("leapsecs_init"),
  m:c("leapsecs_read"), m:c("leapsecs_sub"), m:c("tai_add"), m:c("tai_now"),
  m:c("tai_pack"), m:c("tai_sub"), m:c("tai_unpack"), m:c("taia_add"),
  m:c("taia_approx"), m:c("taia_fmtfrac"), m:c("taia_frac"), m:c("taia_half"),
  m:c("taia_less"), m:c("taia_now"), m:c("taia_pack"), m:c("taia_sub"),
  m:c("taia_tai"), m:c("taia_unpack")
})

m:program("test", {
  m:c("test")
}, {"kumy", "wav", "number", "tai", "m"})

m:program("wav_test", {
  m:c("wav_test")
}, {"wav", "number", "m"})

m:program("lowpass", {
  m:c("lowpass")
}, {"wav", "number", "m"})

m:program("highpass", {
  m:c("highpass")
}, {"wav", "number", "m"})

m:program("fm", {
  m:c("fm")
}, {"wav", "number", "m"})

m:program("kumy2wav", {
  m:c("kumy2wav")
}, {"wav", "number", "kumy", "tai"})

m:program("cos", {
  m:c("cos")
}, {"wav", "number", "m"})

m:program("wav_header", {
  m:c("wav_header")
}, {"wav", "number", "m"})

m:program("seedtest", {
  m:c("seedtest")
}, {"seed", "wav", "number", "tai"})

m:program("wav2seed", {
  m:c("wav2seed")
}, {"seed", "wav", "number", "tai"})

m:program("kumy2seed", {
  m:c("kumy2seed")
}, {"seed", "kumy", "number", "tai"})

m:gen("Makefile")
