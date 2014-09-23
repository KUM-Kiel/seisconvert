#include "caldate.h"

unsigned int caldate_scan(const char *s, struct caldate *cd)
{
  int sign = 1;
  const char *t = s;
  int64_t z;
  int64_t c;

  if (*t == '-') { ++t; sign = -1; }
  z = 0; while ((c = (unsigned char) (*t - '0')) <= 9) { z = z * 10 + c; ++t; }
  cd->year = z * sign;

  if (*t++ != '-') return 0;
  z = 0; while ((c = (unsigned char) (*t - '0')) <= 9) { z = z * 10 + c; ++t; }
  cd->month = z;

  if (*t++ != '-') return 0;
  z = 0; while ((c = (unsigned char) (*t - '0')) <= 9) { z = z * 10 + c; ++t; }
  cd->day = z;

  return t - s;
}
