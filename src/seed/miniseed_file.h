#ifndef MINISEED_FILE_H
#define MINISEED_FILE_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>
#include "seed.h"
#include "taia.h"

/* The structure for storing handles to miniseed-Files. */
typedef struct miniseed_file_s miniseed_file_t;
struct miniseed_file_s {
  /* Userdata. */
  void *data;
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* State machine for reading. */
  uint64_t record_start;
  uint64_t record_size;
  uint64_t frames_in_record;
  uint64_t frame;
  int32_t row[16];
  uint64_t next_row;
  uint64_t row_word;
  uint64_t word_sample;
  int32_t last_sample;
  /* Header and MiniSEED blockette for current record. */
  seed_data_record_header_t record_header;
  seed_blockette_1000_t blockette_1000;
  /* Write buffer for next record. */
  uint8_t *write_buffer;
  uint64_t write_pos;
  struct taia sample_interval;
};

/* Open a MiniSEED file for reading.
 * Supports only big endian files.
 * Supports 32bit sample format and STEIM(1) compression. */
extern miniseed_file_t *miniseed_file_open(const char *path);

/* Creates a MiniSEED file for writing.
 * Before writing frames to the file you MUST call
 * miniseed_file_set_start_time and miniseed_file_set_sample_rate. */
extern miniseed_file_t *miniseed_file_create(const char *path);

/* Closes an open MiniSEED file, frees it's resources and flushes all frames to
 * disk. */
extern void miniseed_file_close(miniseed_file_t *file);

/* Sets the start time of the recording. Must be called exactly once at the
 * beginning on files in write mode. */
extern int miniseed_file_set_start_time(miniseed_file_t *file, struct taia *t);

/* Sets the sample rate of the recording. Must be called exactly once at the
 * beginning on files in write mode. */
extern int miniseed_file_set_sample_rate(miniseed_file_t *file, uint32_t sample_rate);

/* Sets the file info.
 *   station up to 5 characters,
 *   location up to 2 characters,
 *   channel up to 3 characters,
 *   network up to 2 characters.
 * Any field may be shorter or a zero pointer, in which case it gets padded or
 * completely filled with spaces. */
extern int miniseed_file_set_info(miniseed_file_t *file, const char *station, const char *location, const char *channel, const char *network);

/* Reads an int frame from a MiniSEED file in read mode.
 * Returns -1 if there is no more data in the file.
 * Returns -3 if there was an error or the file is malformed. */
extern int miniseed_file_read_int_frame(miniseed_file_t *file, int32_t *frame);
/* Writes an int frame to a MiniSEED file in write mode.
 * Returns -3 in case of an error. */
extern int miniseed_file_write_int_frame(miniseed_file_t *file, const int32_t *frame);

/* Exactly like the int functions, but with doubles.
 * Values range from -1.0 to 1.0. */
extern int miniseed_file_read_double_frame(miniseed_file_t *file, double *frame);
extern int miniseed_file_write_double_frame(miniseed_file_t *file, const double *frame);

#endif
