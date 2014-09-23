#include "leapsecs.h"

struct tai *leapsecs = 0;
int leapsecs_num = 0;

static struct tai t[22] = {
  {.x = 0x4000000004b2580aLL},
  {.x = 0x4000000005a4ec0bLL},
  {.x = 0x4000000007861f8cLL},
  {.x = 0x400000000967530dLL},
  {.x = 0x400000000b48868eLL},
  {.x = 0x400000000d2b0b8fLL},
  {.x = 0x400000000f0c3f10LL},
  {.x = 0x4000000010ed7291LL},
  {.x = 0x4000000012cea612LL},
  {.x = 0x40000000159fca93LL},
  {.x = 0x400000001780fe14LL},
  {.x = 0x4000000019623195LL},
  {.x = 0x400000001d25ea16LL},
  {.x = 0x4000000021dae517LL},
  {.x = 0x40000000259e9d98LL},
  {.x = 0x40000000277fd119LL},
  {.x = 0x400000002a50f59aLL},
  {.x = 0x400000002c32291bLL},
  {.x = 0x400000002e135c9cLL},
  {.x = 0x4000000030e7241dLL},
  {.x = 0x4000000033b8489eLL},
  {.x = 0x40000000368c101fLL}
};
static int n = 22;

int leapsecs_read()
{
  leapsecs = t;
  leapsecs_num = n;
  return 0;
}
