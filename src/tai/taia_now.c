#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include "taia.h"
#include "caltime.h"

/* XXX: breaks tai encapsulation */

void taia_now(struct taia *t)
{
  struct timeval now;
  struct caltime ct;
  time_t nowtime;
  struct tm *nowtm;

  gettimeofday(&now, (struct timezone *) 0);
  nowtime = (time_t)now.tv_sec;
  nowtm = gmtime(&nowtime);

  ct.date.year = nowtm->tm_year + 1900;
  ct.date.month = nowtm->tm_mon;
  ct.date.day = nowtm->tm_mday;
  ct.hour = nowtm->tm_hour;
  ct.minute = nowtm->tm_min;
  ct.second = nowtm->tm_sec;
  ct.offset = 0;

  caltime_tai(&ct, &t->sec);

  t->nano = 1000 * now.tv_usec + 500;
  t->atto = 0;
}
