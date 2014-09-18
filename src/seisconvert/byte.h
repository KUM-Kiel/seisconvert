#ifndef BYTE_H
#define BYTE_H

#include <stdint.h>

extern int byte_copy(uint8_t *dst, uint64_t n, const uint8_t *src);
extern int byte_copy_0(uint8_t *dst, uint64_t n, const uint8_t *src);
extern int byte_set(uint8_t *dst, uint64_t n, uint8_t v);
extern int byte_equal(const uint8_t *a, uint64_t n, const uint8_t *b);

#endif
