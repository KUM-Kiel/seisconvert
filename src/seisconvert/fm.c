#include "fm.h"

#include <math.h>

#define M_TAU 6.283185307179586476925286766559005

double fm_modulate(fm_t *fm, double v)
{
  double w = M_TAU * (fm->f + v * fm->hub);
  fm->phase += w * fm->sample_interval;
  while (fm->phase > M_TAU) {
    fm->phase -= M_TAU;
  }
  while (fm->phase < -M_TAU) {
    fm->phase += M_TAU;
  }
  return cos(fm->phase);
}
