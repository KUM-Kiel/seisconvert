#ifndef FILTER_H
#define FILTER_H

typedef struct biquad_filter_s biquad_filter_t;
struct biquad_filter_s {
  double a[3];
  double b[3];
  double w[2];
};

extern int biquad_filter_init(biquad_filter_t *f, double a0, double a1, double a2, double b1, double b2);

extern int biquad_filter_lowpass(biquad_filter_t *f, double cutoff, double sample_rate);

extern int biquad_filter_highpass(biquad_filter_t *f, double cutoff, double sample_rate);

extern double biquad_filter(biquad_filter_t *f, double x);

#endif
