#ifndef SEED_H
#define SEED_H

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

#endif
