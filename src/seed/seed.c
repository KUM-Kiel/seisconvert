#include "seed.h"

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Shortcut for for loops in standard form. */
#define FOR(i, n) for (i = 0; i < n; ++i)

/* Shortcuts for integer types. */
typedef  uint8_t  u8;
typedef   int8_t  i8;
typedef uint16_t u16;
typedef  int16_t i16;
typedef uint32_t u32;
typedef  int32_t i32;
typedef uint64_t u64;
typedef  int64_t i64;

/*static i16 ld_i16_be(const u8 *x)
{
  i16 i = (i8)x[0];
  return (i << 8) | x[1];
}

static i32 ld_i32_be(const u8 *x)
{
  i32 i = (i8)x[0];
  i = (i << 8) | x[1];
  i = (i << 8) | x[2];
  return (i << 8) | x[3];
}*/

/* The structure for storing handles to SEED-Files. */
struct seedfile_s {
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
};

seedfile_t *seed_open(const char *path)
{
  seedfile_t *file = malloc(sizeof(seedfile_t));
  if (!file) return 0;
  file->file_handle = fopen(path, "r");
  if (!file->file_handle) goto err0;
  file->mode = 0;
  return file;
  /* Undo everything in case of an error. */
err0:
  free(file);
  return 0;
}

seedfile_t *seed_create(const char *path)
{
  seedfile_t *file = malloc(sizeof(seedfile_t));
  if (!file) return 0;
  file->file_handle = fopen(path, "w");
  if (!file->file_handle) goto err0;
  file->mode = 1;
  return file;
  /* Undo everything in case of an error. */
err0:
  free(file);
  return 0;
}

void seed_close(seedfile_t *file)
{
  if (!file) return;
  fclose(file->file_handle);
  free(file);
}
