#ifndef FM_H
#define FM_H

typedef struct fm_s fm_t;
struct fm_s {
  double f;     /* Grundfrequenz in Hz */
  double hub;   /* Frequenzhub in Hz */
  double phase; /* Phase */
  double sample_interval;
};

extern double fm_modulate(fm_t *fm, double v);

#endif
