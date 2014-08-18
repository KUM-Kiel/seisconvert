#ifndef SEED_H
#define SEED_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>

#include "taia.h"

/* The structure for storing handles to SEED-Files. */
typedef struct seedfile_s seedfile_t;
struct seedfile_s {
  /* Userdata. */
  void *data;
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* Indicates end of file. */
  int eof;
};

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

typedef struct {
  int64_t sequence_number;
  uint8_t station_identifier[6];
  uint8_t location_identifier[3];
  uint8_t channel_identifier[4];
  uint8_t network_code[3];
  struct taia start_time;
  uint16_t num_samples;
  int16_t sample_rate_factor;
  int16_t sample_rate_multiplier;
  uint8_t activity_flags;
  uint8_t io_flags;
  uint8_t data_quality_flags;
  uint8_t blockette_count;
  int32_t time_correction;
  uint16_t data_offset;
  uint16_t blockette_offset;
} data_record_header;

/* Reads a data record header. */
extern int read_data_record_header(data_record_header *h, const uint8_t *x);

typedef struct {
  uint16_t next_blockette;
  uint8_t encoding;
  uint8_t word_order;
  uint8_t data_record_length;
} blockette_1000;

extern int read_blockette_1000(blockette_1000 *b, const uint8_t *x);

#endif
