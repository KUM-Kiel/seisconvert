local make = require("make")

local m = make()

m:lib("kumy", {
  m:c("kumy", {"kumy.h"})
})

m:lib("seed", {
  m:c("seed", {"seed.h", "../buffer/buffer.h", "../buffer/number.h", "../tai/taia.h"})
})

m:lib("segy", {
  m:c("segy", {"segy.h"})
})

m:lib("wav", {
  m:c("wav", {"wav.h"}),
  m:c("filter", {"filter.h"}),
  m:c("fm", {"fm.h"})
})

m:lib("buffer", {
  m:c("buffer", {"buffer.h"}),
  m:c("number", {"number.h"})
})

m:lib("tai", {
  m:c("caldate_fmjd", {"caldate.h"}),
  m:c("caldate_fmt", {"caldate.h"}),
  m:c("caldate_mjd", {"caldate.h"}),
  m:c("caldate_norm", {"caldate.h"}),
  m:c("caldate_scan", {"caldate.h"}),
  m:c("caldate_ster", {"caldate.h"}),
  m:c("caltime_fmt", {"caldate.h", "caltime.h"}),
  m:c("caltime_scan", {"caldate.h", "caltime.h"}),
  m:c("caltime_tai", {"caldate.h", "caltime.h", "tai.h", "uint64.h", "leapsecs.h"}),
  m:c("caltime_utc", {"caldate.h", "caltime.h", "tai.h", "uint64.h", "leapsecs.h"}),
  m:c("leapsecs_add", {"leapsecs.h", "tai.h", "uint64.h"}),
  m:c("leapsecs_init", {"leapsecs.h", "tai.h", "uint64.h"}),
  m:c("leapsecs_read", {"leapsecs.h", "tai.h", "uint64.h"}),
  m:c("leapsecs_sub", {"leapsecs.h", "tai.h", "uint64.h"}),
  m:c("tai_add", {"tai.h", "uint64.h"}),
  m:c("tai_now", {"tai.h", "uint64.h"}),
  m:c("tai_pack", {"tai.h", "uint64.h"}),
  m:c("tai_sub", {"tai.h", "uint64.h"}),
  m:c("tai_unpack", {"tai.h", "uint64.h"}),
  m:c("taia_add", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_approx", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_fmtfrac", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_frac", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_half", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_less", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_now", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_pack", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_sub", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_tai", {"taia.h", "tai.h", "uint64.h"}),
  m:c("taia_unpack", {"taia.h", "tai.h", "uint64.h"})
})

m:program("test", {
  m:c("test", {"kumy/kumy.h", "wav/wav.h", "wav/fm.h"})
}, {"kumy", "wav", "tai", "m"})

m:program("wav_test", {
  m:c("wav_test", {"wav/wav.h"})
}, {"wav", "m"})

m:program("lowpass", {
  m:c("lowpass", {"wav/wav.h", "wav/filter.h"})
}, {"wav", "m"})

m:program("highpass", {
  m:c("highpass", {"wav/wav.h", "wav/filter.h"})
}, {"wav", "m"})

m:program("fm", {
  m:c("fm", {"wav/wav.h", "wav/fm.h"})
}, {"wav", "m"})

m:program("kumy2wav", {
  m:c("kumy2wav", {"wav/wav.h", "kumy/kumy.h"})
}, {"wav", "kumy"})

m:program("cos", {
  m:c("cos", {"wav/wav.h"})
}, {"wav", "m"})

m:program("seedtest", {
  m:c("seedtest", {"seed/seed.h"})
}, {"seed", "buffer", "tai"})

m:gen("Makefile")
