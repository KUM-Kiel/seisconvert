#include "tai.h"

#define GPS_EPOCH 4611686018743352723LL

int64_t tai_gps_sec(const struct tai *t)
{
  return t->x - GPS_EPOCH;
}

void tai_from_gps(struct tai *t, int64_t sec)
{
  t->x = sec + GPS_EPOCH;
}
