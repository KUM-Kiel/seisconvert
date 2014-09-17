#ifndef KUMY_FILE_H
#define KUMY_FILE_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>

#include "kumy.h"

#define KUMY_FILE_CHANNELS 4

/* The structure for storing handles to KUMY files. */
typedef struct kumy_file_s kumy_file_t;
struct kumy_file_s {
  /* Userdata. */
  void *data;
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* KUMY header. */
  kumy_text_header_t text_header[KUMY_FILE_CHANNELS];
  kumy_binary_header_t binary_header[KUMY_FILE_CHANNELS];
  /* KUMY frame config. */
  kumy_frame_config_t frame_config;
  /* Frame buffer. */
  uint8_t *buffer;
};

/* Opens a KUMY file for reading. */
extern kumy_file_t *kumy_file_open(const char *path);

/* Creates a KUMY file for writing with the given sample rate, number of channels
 * and bit depth. */
extern kumy_file_t *kumy_file_create(const char *path, uint32_t sample_rate);

/* Closes an open KUMY file, frees it's resources and writes all pending frames
 * to disk. */
extern void kumy_file_close(kumy_file_t *file);

/* Reads a frame from the KUMY file and stores it as 32 bit integers.
 * The supplied frame pointer must have enough space for one int32_t per channel.
 * Returns -1 if there is no more data in the file.
 * Returns -3 if there was an error or the file is malformed. */
extern int kumy_file_read_int_frame(kumy_file_t *file, int32_t *frame);

/* Writes a frame to the KUMY file. The values are taken from the supplied frame
 * pointer, one int32_t per channel. */
extern int kumy_file_write_int_frame(kumy_file_t *file, const int32_t *frame);

/* Exactly like the int functions, but with doubles.
 * The values range from -1.0 to 1.0. */
extern int kumy_file_read_double_frame(kumy_file_t *file, double *frame);
extern int kumy_file_write_double_frame(kumy_file_t *file, const double *frame);

#endif
