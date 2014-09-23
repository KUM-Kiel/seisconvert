#ifndef CALTIME_H
#define CALTIME_H

#include "caldate.h"
#include "tai.h"

struct caltime {
  struct caldate date;
  int64_t hour;
  int64_t minute;
  int64_t second;
  int64_t offset;
};

extern void caltime_tai(const struct caltime *ct, struct tai *t);
extern void caltime_utc(struct caltime *ct, const struct tai *t, int *pwday, int *pyday);

extern unsigned int caltime_fmt(char *s, const struct caltime *ct);
extern unsigned int caltime_scan(const char *s, struct caltime *ct);

#endif
