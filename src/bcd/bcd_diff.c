#include "bcd.h"

long long bcd_diff(const char *t1, const char *t2)
{
  long long d = bcd_mjd(t2) - bcd_mjd(t1);
  d *= 24;
  d += bcd_int(t2[BCD_HOUR]) - bcd_int(t1[BCD_HOUR]);
  d *= 60;
  d += bcd_int(t2[BCD_MINUTE]) - bcd_int(t1[BCD_MINUTE]);
  d *= 60;
  d += bcd_int(t2[BCD_SECOND]) - bcd_int(t1[BCD_SECOND]);
  return d;
}
