#ifndef CALDATE_H
#define CALDATE_H

#include <stdint.h>

struct caldate {
  int64_t year;
  int64_t month;
  int64_t day;
};

extern unsigned int caldate_fmt(char *s, const struct caldate *cd);
extern unsigned int caldate_scan(const char *s, struct caldate *cd);

extern void caldate_frommjd(struct caldate *cd, int64_t day, int *pwday, int *pyday);
extern int64_t caldate_mjd(const struct caldate *cd);
extern void caldate_normalize(struct caldate *cd);

extern void caldate_easter(struct caldate *cd);

#endif
