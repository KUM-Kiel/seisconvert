local leapdat = io.open("leapsecs.dat", "r"):read("*a")
local f = io.open("leapsecs_read.c", "w")

f:write([[#include "tai.h"
#include "leapsecs.h"

struct tai *leapsecs = 0;
int leapsecs_num = 0;

]])

f:write("static const unsigned char leapsecs_dat[] = {\n")
for i = 1, #leapdat do
  if i % 8 == 1 then
    f:write("  ")
  end
  f:write(string.format("0x%02x", leapdat:byte(i)))
  if i == #leapdat then
    f:write("\n")
  elseif i % 8 == 0 then
    f:write(",\n")
  else
    f:write(", ")
  end
end
f:write("};\n\n")

f:write("static struct tai t[" .. (#leapdat / 8) .. "];\n\n")

f:write([[int leapsecs_read()
{
  int i, n;
  struct tai u;

  n = ]] .. (#leapdat / 8) .. [[;

  for (i = 0;i < n;++i) {
    tai_unpack(leapsecs_dat + 8 * i,&u);
    t[i] = u;
  }

  leapsecs = t;
  leapsecs_num = n;
  return 0;
}
]])
