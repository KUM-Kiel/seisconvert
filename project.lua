local make = require("make")

local m = make()

m:lib("seisconvert", {
  m:c("kum_segy", {"kum_segy.h"}),
  m:c("wav", {"wav.h"}),
  m:c("fm", {"fm.h"}),
  m:c("filter", {"filter.h"}),
  m:c("segy", {"segy.h"})
})

m:program("test", {
  m:c("test", {"seisconvert/kum_segy.h", "seisconvert/wav.h", "seisconvert/fm.h"})
}, {"seisconvert", "m"})

m:program("wav_test", {
  m:c("wav_test", {"seisconvert/wav.h"})
}, {"seisconvert", "m"})

m:program("lowpass", {
  m:c("lowpass", {"seisconvert/wav.h"})
}, {"seisconvert", "m"})

m:program("highpass", {
  m:c("highpass", {"seisconvert/wav.h"})
}, {"seisconvert", "m"})

m:gen("Makefile")
