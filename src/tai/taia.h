#ifndef TAIA_H
#define TAIA_H

#include "tai.h"

struct taia {
  struct tai sec;
  uint32_t nano; /* 0...999999999 */
  uint32_t atto; /* 0...999999999 */
};

extern void taia_tai(const struct taia *ta, struct tai *t);

extern void taia_now(struct taia *t);

extern double taia_approx(const struct taia *t);
extern double taia_frac(const struct taia *t);

extern void taia_add(struct taia *t, const struct taia *u, const struct taia *v);
extern void taia_sub(struct taia *t, const struct taia *u, const struct taia *v);
extern void taia_half(struct taia *t, const struct taia *u);
extern int taia_less(const struct taia *t, const struct taia *u);

#define TAIA_PACK 16
extern void taia_pack(char *s, const struct taia *t);
extern void taia_unpack(const char *s, struct taia *t);

#define TAIA_FMTFRAC 19
extern unsigned int taia_fmtfrac(char *s, const struct taia *t);

#endif
