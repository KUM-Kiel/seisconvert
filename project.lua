local make = require("make")

local m = make()

m:lib("kumy", {
  m:c("kumy", {"kumy.h"})
})

m:lib("segy", {
  m:c("segy", {"segy.h"})
})

m:lib("wav", {
  m:c("wav", {"wav.h"}),
  m:c("filter", {"filter.h"}),
  m:c("fm", {"fm.h"})
})

m:program("test", {
  m:c("test", {"kumy/kumy.h", "wav/wav.h", "wav/fm.h"})
}, {"kumy", "wav", "m"})

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

m:program("kumy", {
  m:c("kumy", {"wav/wav.h", "kumy/kumy.h"})
}, {"wav", "kumy"})

m:program("cos", {
  m:c("cos", {"wav/wav.h"})
}, {"wav", "m"})

m:gen("Makefile")
