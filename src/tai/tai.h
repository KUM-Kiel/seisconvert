#ifndef TAI_H
#define TAI_H

#include <stdint.h>

struct tai {
  int64_t x;
};

extern void tai_now(struct tai *t);

#define tai_approx(t) ((double) ((t)->x))

extern void tai_add(struct tai *t, const struct tai *u, const struct tai *v);
extern void tai_sub(struct tai *t, const struct tai *u, const struct tai *v);
#define tai_less(t, u) ((t)->x < (u)->x)

#define TAI_PACK 8
extern void tai_pack(char *s, const struct tai *t);
extern void tai_unpack(const char *s, struct tai *t);

extern int64_t tai_gps_sec(const struct tai *t);
extern void tai_from_gps(struct tai *t, int64_t sec);

#endif
