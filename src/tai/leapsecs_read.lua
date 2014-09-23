local leapdat = io.open("leapsecs.dat", "r"):read("*a")
local f = io.open("leapsecs_read.c", "w")

f:write([[#include "leapsecs.h"

struct tai *leapsecs = 0;
int leapsecs_num = 0;

]])

f:write("static struct tai t[" .. (#leapdat / 8) .. "] = {\n")
for i = 1, #leapdat do
  if i % 8 == 1 then
    f:write("  {.x = 0x")
  end
  f:write(string.format("%02x", leapdat:byte(i)))
  if i == #leapdat then
    f:write("LL}\n")
  elseif i % 8 == 0 then
    f:write("LL},\n")
  else
    f:write("")
  end
end
f:write("};\n")

f:write([[static int n = ]] .. (#leapdat / 8) .. [[;

int leapsecs_read()
{
  leapsecs = t;
  leapsecs_num = n;
  return 0;
}
]])
