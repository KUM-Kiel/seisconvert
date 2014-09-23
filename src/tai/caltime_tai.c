#include "tai.h"
#include "leapsecs.h"
#include "caltime.h"

/* XXX: breaks tai encapsulation */
void caltime_tai(const struct caltime *ct, struct tai *t)
{
  int64_t day;
  int64_t s;

  /* XXX: check for overflow? */

  day = caldate_mjd(&ct->date);

  s = ct->hour * 60 + ct->minute;
  s = (s - ct->offset) * 60 + ct->second;

  t->x = day * 86400LL + 4611686014920671114LL + s;

  leapsecs_add(t, ct->second == 60);
}
