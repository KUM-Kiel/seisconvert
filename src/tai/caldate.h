#ifndef CALDATE_H
#define CALDATE_H

#include <stdint.h>

struct caldate {
  long year;
  int month;
  int day;
} ;

extern unsigned int caldate_fmt();
extern unsigned int caldate_scan();

extern void caldate_frommjd();
extern long caldate_mjd();
extern void caldate_normalize();

extern void caldate_easter();

#endif
