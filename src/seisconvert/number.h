#ifndef NUMBER_H
#define NUMBER_H

#include <stdint.h>

/* Read signed or unsigned 16, 32 or 64 bit integers
 * from a buffer in big endian (aka Motorola format). */
extern  int16_t ld_i16_be(const uint8_t *x);
extern  int32_t ld_i32_be(const uint8_t *x);
extern  int64_t ld_i64_be(const uint8_t *x);
extern uint16_t ld_u16_be(const uint8_t *x);
extern uint32_t ld_u32_be(const uint8_t *x);
extern uint64_t ld_u64_be(const uint8_t *x);

/* Read signed or unsigned 16, 32 or 64 bit integers
 * from a buffer in little endian (aka Intel format). */
extern  int16_t ld_i16_le(const uint8_t *x);
extern  int32_t ld_i32_le(const uint8_t *x);
extern  int64_t ld_i64_le(const uint8_t *x);
extern uint16_t ld_u16_le(const uint8_t *x);
extern uint32_t ld_u32_le(const uint8_t *x);
extern uint64_t ld_u64_le(const uint8_t *x);

/* Write signed or unsigned 16, 32 or 64 bit integers
 * to a buffer in big endian. */
extern void st_i16_be(uint8_t *x,  int16_t i);
extern void st_i32_be(uint8_t *x,  int32_t i);
extern void st_i64_be(uint8_t *x,  int64_t i);
extern void st_u16_be(uint8_t *x, uint16_t u);
extern void st_u32_be(uint8_t *x, uint32_t u);
extern void st_u64_be(uint8_t *x, uint64_t u);

/* Write signed or unsigned 16, 32 or 64 bit integers
 * to a buffer in little endian. */
extern void st_i16_le(uint8_t *x,  int16_t i);
extern void st_i32_le(uint8_t *x,  int32_t i);
extern void st_i64_le(uint8_t *x,  int64_t i);
extern void st_u16_le(uint8_t *x, uint16_t u);
extern void st_u32_le(uint8_t *x, uint32_t u);
extern void st_u64_le(uint8_t *x, uint64_t u);

/* Write an integer into a string in decimal representation.
 * The number will be right aligned and exactly xlen bytes long.
 * If pad is nonzero, the number will be padded with zeros instead of spaces.
 * If the number is negative, there will be a dash (-) in front of it.
 * Returns zero, if everything went right. If the numbers magnitude is to large,
 * only the least significant xlen digits will be written and -1 is returned. */
extern int write_int(uint8_t *x, uint64_t xlen, int64_t i, int pad);

/* Reads a number from a string. The number should be exactly xlen digits long,
 * though leading spaces or zeros are allowed.
 * If the number is well formed, it will be stored to i and 0 is returned.
 * Else -1 will be returned and i is left untouched. */
extern int parse_int(int64_t *i, const uint8_t *x, uint64_t xlen);

#endif
