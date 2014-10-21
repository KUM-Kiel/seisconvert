#include "bcd.h"

static const int times365[4] = {0, 365, 730, 1095};
static const int montab[12] = {0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};
long bcd_mjd(const char *bcd)
{
  long d, m, y;
  d = bcd_int(bcd[BCD_DAY]);
  m = bcd_int(bcd[BCD_MONTH]) - 1;
  y = bcd_int(bcd[BCD_YEAR]);

  if (m >= 2) {
    m -= 2;
  } else {
    m += 10;
    --y;
  }

  d += montab[m];

  d += times365[y & 3];
  y >>= 2;

  d += 1461L * y;

  return d + 59;
}
