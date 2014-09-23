#include "tai.h"
#include "leapsecs.h"
#include "caltime.h"

/* XXX: breaks tai encapsulation */

void caltime_utc(struct caltime *ct, const struct tai *t, int *pwday, int *pyday)
{
  struct tai t2 = *t;
  int64_t u;
  int leap;
  int64_t s;

  /* XXX: check for overfow? */

  leap = leapsecs_sub(&t2);
  u = t2.x;

  u += 58486;
  s = u % 86400ULL;

  ct->second = (s % 60) + leap; s /= 60;
  ct->minute = s % 60; s /= 60;
  ct->hour = s;

  u /= 86400ULL;
  caldate_frommjd(&ct->date, /*XXX*/(u - 53375995543064LL), pwday, pyday);

  ct->offset = 0;
}
