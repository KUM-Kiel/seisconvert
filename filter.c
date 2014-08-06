#include "filter.h"

#include <math.h>

int biquad_filter_init(biquad_filter_t *f, double a0, double a1, double a2, double b1, double b2)
{
  if (!f) return -1;
  f->a[0] = a0; f->a[1] = a1; f->a[2] = a2;
  f->b[0] = 1;  f->b[1] = b1; f->b[2] = b2;
  f->w[0] = 0;  f->w[1] = 0;
  return 0;
}

int biquad_filter_lowpass(biquad_filter_t *f, double cutoff, double sample_rate)
{
  double a0, a1, a2, b1, b2, norm, K, Q;
  Q = pow(2, -0.5);
  K = tan(M_PI * cutoff / sample_rate);
  norm = 1 / (1 + K / Q + K * K);
  a0 = K * K * norm;
  a1 = 2 * a0;
  a2 = a0;
  b1 = 2 * (K * K - 1) * norm;
  b2 = (1 - K / Q + K * K) * norm;
  return biquad_filter_init(f, a0, a1, a2, b1, b2);
}

int biquad_filter_highpass(biquad_filter_t *f, double cutoff, double sample_rate)
{
  double a0, a1, a2, b1, b2, norm, K, Q;
  Q = pow(2, -0.5);
  K = tan(M_PI * cutoff / sample_rate);
  norm = 1 / (1 + K / Q + K * K);
  a0 = 1 * norm;
  a1 = -2 * a0;
  a2 = a0;
  b1 = 2 * (K * K - 1) * norm;
  b2 = (1 - K / Q + K * K) * norm;
  return biquad_filter_init(f, a0, a1, a2, b1, b2);
}

double biquad_filter(biquad_filter_t *f, double x)
{
  double w, y;
  w = f->a[0] * x + f->a[1] * f->w[0] + f->a[2] * f->w[1];
  y = f->b[0] * w - f->b[1] * f->w[0] - f->b[2] * f->w[1];
  /* Update state */
  f->w[1] = f->w[0]; f->w[0] = w;
  return y;
}
