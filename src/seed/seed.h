#ifndef SEED_H
#define SEED_H

#include <stdint.h>

/* Opaque structure for SEED files.
 * Use the methods below to work with these. */
typedef struct seedfile_s seedfile_t;

/* Open a SEED file for reading.
 * Returns a SEED file handle.
 * Returns NULL in case of an error. */
extern seedfile_t *seed_open(const char *path);

/* Creates a SEED file and opens it for writing.
 * Returns the file handle if everything went right.
 * Returns NULL in case of an error. */
extern seedfile_t *seed_create(const char *path);

/* Closes an open SEED file and frees the associated handle. */
extern void seed_close(seedfile_t *file);

typedef struct seed_buffer_s seed_buffer_t;
struct seed_buffer_s {
  uint8_t *data;
  uint64_t size;
};

typedef void (*seed_data_cb)(seedfile_t *f, int64_t size, seed_buffer_t *buf);
typedef void (*seed_samples_cb)(seedfile_t *f, double *data, uint64_t n);
typedef void (*seed_alloc_cb)(seedfile_t *f, uint64_t required_size, seed_buffer_t *buf);

extern int seed_begin_read(seedfile_t *file, seed_data_cb data_cb, seed_alloc_cb alloc_cb);
extern int seed_stop_read(seedfile_t *file);

#endif
