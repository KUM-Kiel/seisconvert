#include "bcd.h"

#define mod(a, b) ((((a) % (b)) + (b)) % (b))
#define mod1(a, b) (mod((a) - 1, b) + 1)
int bcd_weekday(const char *bcd)
{
  return mod1(bcd_mjd(bcd), 7);
}
