#include <time.h>
#include "tai.h"

void tai_now(t)
struct tai *t;
{
  t->x = 4611686018427387914LL + (int64_t) time((long *) 0);
}
