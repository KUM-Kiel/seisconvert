#include <time.h>
#include "tai.h"
#include "caltime.h"

void tai_now(struct tai *t)
{
  struct caltime ct;
  time_t nowtime;
  struct tm *nowtm;

  time(&nowtime);
  nowtm = gmtime(&nowtime);

  ct.date.year = nowtm->tm_year + 1900;
  ct.date.month = nowtm->tm_mon;
  ct.date.day = nowtm->tm_mday;
  ct.hour = nowtm->tm_hour;
  ct.minute = nowtm->tm_min;
  ct.second = nowtm->tm_sec;
  ct.offset = 0;

  caltime_tai(&ct, t);
}
