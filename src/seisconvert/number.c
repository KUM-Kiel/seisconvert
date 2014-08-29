#include "number.h"

/* Shortcuts for integer types. */
typedef  uint8_t  u8;
typedef   int8_t  i8;
typedef uint16_t u16;
typedef  int16_t i16;
typedef uint32_t u32;
typedef  int32_t i32;
typedef uint64_t u64;
typedef  int64_t i64;

/* Load BE */

i16 ld_i16_be(const u8 *x)
{
  i16 i = (i8)x[0];
  return (i << 8) | x[1];
}

i32 ld_i32_be(const u8 *x)
{
  i32 i = (i8)x[0];
  i = (i << 8) | x[1];
  i = (i << 8) | x[2];
  return (i << 8) | x[3];
}

i64 ld_i64_be(const u8 *x)
{
  i64 i = (i8)x[0];
  i = (i << 8) | x[1];
  i = (i << 8) | x[2];
  i = (i << 8) | x[3];
  i = (i << 8) | x[4];
  i = (i << 8) | x[5];
  i = (i << 8) | x[6];
  return (i << 8) | x[7];
}

u16 ld_u16_be(const u8 *x)
{
  u16 u = x[0];
  return (u << 8) | x[1];
}

u32 ld_u32_be(const u8 *x)
{
  u32 u = x[0];
  u = (u << 8) | x[1];
  u = (u << 8) | x[2];
  return (u << 8) | x[3];
}

u64 ld_u64_be(const u8 *x)
{
  u64 u = x[0];
  u = (u << 8) | x[1];
  u = (u << 8) | x[2];
  u = (u << 8) | x[3];
  u = (u << 8) | x[4];
  u = (u << 8) | x[5];
  u = (u << 8) | x[6];
  return (u << 8) | x[7];
}

/* Load LE */

i16 ld_i16_le(const u8 *x)
{
  i16 i = (i8)x[1];
  return (i << 8) | x[0];
}

i32 ld_i32_le(const u8 *x)
{
  i32 i = (i8)x[3];
  i = (i << 8) | x[2];
  i = (i << 8) | x[1];
  return (i << 8) | x[0];
}

i64 ld_i64_le(const u8 *x)
{
  i64 i = (i8)x[7];
  i = (i << 8) | x[6];
  i = (i << 8) | x[5];
  i = (i << 8) | x[4];
  i = (i << 8) | x[3];
  i = (i << 8) | x[2];
  i = (i << 8) | x[1];
  return (i << 8) | x[0];
}

u16 ld_u16_le(const u8 *x)
{
  u16 u = x[1];
  return (u << 8) | x[0];
}

u32 ld_u32_le(const u8 *x)
{
  u32 u = x[3];
  u = (u << 8) | x[2];
  u = (u << 8) | x[1];
  return (u << 8) | x[0];
}

u64 ld_u64_le(const u8 *x)
{
  u64 u = x[7];
  u = (u << 8) | x[6];
  u = (u << 8) | x[5];
  u = (u << 8) | x[4];
  u = (u << 8) | x[3];
  u = (u << 8) | x[2];
  u = (u << 8) | x[1];
  return (u << 8) | x[0];
}

/* Store BE */

void st_i16_be(u8 *x, i16 i)
{
  x[1] = i; i >>= 8;
  x[0] = i;
}

void st_i32_be(u8 *x, i32 i)
{
  x[3] = i; i >>= 8;
  x[2] = i; i >>= 8;
  x[1] = i; i >>= 8;
  x[0] = i;
}

void st_i64_be(u8 *x, i64 i)
{
  x[7] = i; i >>= 8;
  x[6] = i; i >>= 8;
  x[5] = i; i >>= 8;
  x[4] = i; i >>= 8;
  x[3] = i; i >>= 8;
  x[2] = i; i >>= 8;
  x[1] = i; i >>= 8;
  x[0] = i;
}

void st_u16_be(u8 *x, u16 u)
{
  x[1] = u; u >>= 8;
  x[0] = u;
}

void st_u32_be(u8 *x, u32 u)
{
  x[3] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[0] = u;
}

void st_u64_be(u8 *x, u64 u)
{
  x[7] = u; u >>= 8;
  x[6] = u; u >>= 8;
  x[5] = u; u >>= 8;
  x[4] = u; u >>= 8;
  x[3] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[0] = u;
}

/* Store LE */

void st_i16_le(u8 *x, i16 i)
{
  x[0] = i; i >>= 8;
  x[1] = i;
}

void st_i32_le(u8 *x, i32 i)
{
  x[0] = i; i >>= 8;
  x[1] = i; i >>= 8;
  x[2] = i; i >>= 8;
  x[3] = i;
}

void st_i64_le(u8 *x, i64 i)
{
  x[0] = i; i >>= 8;
  x[1] = i; i >>= 8;
  x[2] = i; i >>= 8;
  x[3] = i; i >>= 8;
  x[4] = i; i >>= 8;
  x[5] = i; i >>= 8;
  x[6] = i; i >>= 8;
  x[7] = i;
}

void st_u16_le(u8 *x, u16 u)
{
  x[0] = u; u >>= 8;
  x[1] = u;
}

void st_u32_le(u8 *x, u32 u)
{
  x[0] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[3] = u;
}

void st_u64_le(u8 *x, u64 u)
{
  x[0] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[3] = u; u >>= 8;
  x[4] = u; u >>= 8;
  x[5] = u; u >>= 8;
  x[6] = u; u >>= 8;
  x[7] = u;
}

int write_int(uint8_t *x, uint64_t xlen, int64_t i, int pad)
{
  int p = 1, z = 1;
  if (i < 0) {
    p = 0;
    i = -i;
  }
  while (xlen > 0) {
    --xlen;
    if (p || xlen > 0) {
      if (i) {
        x[xlen] = (i % 10) + '0';
        i /= 10;
      } else {
        x[xlen] = z ? '0' : ' ';
      }
    } else {
      x[xlen] = '-';
    }
    z = pad;
  }
  return i ? -1 : 0;
}

int parse_int(int64_t *i, const uint8_t *x, uint64_t xlen)
{
  int p = 1, s = 0, j;
  int64_t t = 0;
  if (x[0] == '-') p = 0;
  for (j = 1 - p; j < xlen; ++j) {
    if ('0' <= x[j] && x[j] <= '9') {
      t = t * 10 + x[j] - '0';
      s = 1;
    } else if (x[j] != ' ' || s) {
      return -1;
    }
  }
  *i = p ? t : -t;
  return 0;
}
