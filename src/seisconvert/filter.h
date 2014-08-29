#ifndef FILTER_H
#define FILTER_H

/* Structure for biquad filters. */
typedef struct biquad_filter_s biquad_filter_t;
struct biquad_filter_s {
  /* Parameter. */
  double a[3];
  /* b[0] is ignored. */
  double b[3];
  /* Internal state. */
  double w[2];
};

/* Initialises the given biquad_filter_t structure with the given parameters
 * and clears the internal state. */
extern int biquad_filter_init(biquad_filter_t *f, double a0, double a1, double a2, double b1, double b2);

/* Convinience function for a lowpass filter.
 * Initalises the given biquad_filter_t structure as a lowpass with the given
 * cutoff frequency and sample rate in Hz. */
extern int biquad_filter_lowpass(biquad_filter_t *f, double cutoff, double sample_rate);

/* Convinience function for a highpass filter.
 * Initalises the given biquad_filter_t structure as a highpass with the given
 * cutoff frequency and sample rate in Hz. */
extern int biquad_filter_highpass(biquad_filter_t *f, double cutoff, double sample_rate);

/* Filters the sample x with the given biquad_filter_t structure.
 * Returns the filtered sample.
 * Call this functionon a signal sample by sample. */
extern double biquad_filter(biquad_filter_t *f, double x);

#endif
