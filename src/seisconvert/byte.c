#include "byte.h"

int byte_copy(uint8_t *dst, uint64_t n, const uint8_t *src)
{
  uint64_t i;
  for (i = 0; i < n; ++i) {
    dst[i] = src[i];
  }
  return 0;
}

int byte_copy_0(uint8_t *dst, uint64_t n, const uint8_t *src)
{
  uint64_t i;
  for (i = 0; i < n; ++i) {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return 0;
}

int byte_set(uint8_t *dst, uint64_t n, uint8_t v)
{
  uint64_t i;
  for (i = 0; i < n; ++i) {
    dst[i] = v;
  }
  return 0;
}

int byte_equal(const uint8_t *a, uint64_t n, const uint8_t *b)
{
  uint64_t i, r = 0;
  for (i = 0; i < n; ++i) {
    r |= a[i] ^ b[i];
  }
  return !r;
}
