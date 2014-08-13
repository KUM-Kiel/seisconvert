#ifndef SEED_H
#define SEED_H

typedef struct seedfile_s seedfile_t;

extern seedfile_t *seed_open(const char *path);
extern void seed_close(seedfile_t *file);

#endif
