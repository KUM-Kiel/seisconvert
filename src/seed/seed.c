#include "seed.h"

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "buffer.h"
#include "number.h"

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

/* The structure for storing handles to SEED-Files. */
struct seedfile_s {
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* Buffer for caching data. */
  buffer_t *buffer;
};

#define BUFFER_SIZE 4096

seedfile_t *seed_open(const char *path)
{
  /* Allocate memory for the handle. */
  seedfile_t *file = malloc(sizeof(seedfile_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "r"))) goto err1;
  file->mode = 0;
  /* Create buffer. */
  if (!(file->buffer = buffer_new(BUFFER_SIZE))) goto err2;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

seedfile_t *seed_create(const char *path)
{
  /* Allocate memory for the handle. */
  seedfile_t *file = malloc(sizeof(seedfile_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "w"))) goto err1;
  file->mode = 1;
  /* Create buffer. */
  if (!(file->buffer = buffer_new(BUFFER_SIZE))) goto err2;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

void seed_close(seedfile_t *file)
{
  if (!file) return;
  buffer_delete(file->buffer);
  fclose(file->file_handle);
  free(file);
}
