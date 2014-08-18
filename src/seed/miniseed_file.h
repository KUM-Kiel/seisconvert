#ifndef MINISEED_FILE_H
#define MINISEED_FILE_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>
#include "seed.h"

/* The structure for storing handles to miniseed-Files. */
typedef struct miniseed_file_s miniseed_file_t;
struct miniseed_file_s {
  /* Userdata. */
  void *data;
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* Indicates end of file. */
  int eof;
  /* Frame buffer. */
  uint8_t *buffer;
};

extern miniseed_file_t *miniseed_file_open(const char *path);
extern miniseed_file_t *miniseed_file_create(const char *path);
extern void miniseed_file_close(miniseed_file_t *file);

extern int miniseed_file_read_int_frame(miniseed_file_t *file, int32_t *frame);
extern int miniseed_file_write_int_frame(miniseed_file_t *file, const int32_t *frame);

extern int miniseed_file_read_double_frame(miniseed_file_t *file, double *frame);
extern int miniseed_file_write_double_frame(miniseed_file_t *file, const double *frame);

#endif
