#include "seed.h"

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "buffer.h"
#include "number.h"
#include "taia.h"
#include "caltime.h"

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
  /* Indicates end of file. */
  int eof;
};

/* Copy l bytes from `from` to `to`. */
static void byte_copy(u8 *to, u64 l, u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
}

/* Copy l bytes from `from` to `to`. Appends 0 byte to `to`. */
static void byte_copy_0(u8 *to, u64 l, u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
  to[i] = 0;
}

/* Return the next record type. The buffer in f must be positioned at the
 * beginning of a record. Returns -1 if there is not enough data. Returns -3 if
 * the buffer is not at a record border. Returns 'D' for a data record, 'V' for
 * a volume header, 'A' for a dictionary header, 'S' for a station header and
 * 'T' for a time span header. Returns a lowercase letter if the record
 * continues the last one. */
static int next_record(seedfile_t *f)
{
  u8 x[8];
  i64 no;
  if (buffer_read(f->buffer, x, 8) < 8) return -1;
  if (parse_int(&no, x, 6) < 0) return -3;
  if (no < 0) return -3;
  if (x[7] == ' ') {
    switch (x[6]) {
      case 'D': return 'D';
      case 'V': return 'V';
      case 'A': return 'A';
      case 'S': return 'S';
      case 'T': return 'T';
      default: return -3;
    }
  } else if (x[7] == '*') {
    switch (x[6]) {
      case 'V': return 'v';
      case 'A': return 'a';
      case 'S': return 's';
      case 'T': return 't';
      default: return -3;
    }
  }
  return -3;
}

/* Reads the time in x coded as BTIME and stores it in t as struct taia. */
static int read_btime(struct taia *t, const u8 *x)
{
  long h_us;
  struct caltime ct;
  ct.date.year = ld_u16_be(x);
  ct.date.month = 1;
  ct.date.day = ld_u16_be(x + 2);
  ct.hour = x[4];
  ct.minute = x[5];
  ct.second = x[6];
  ct.offset = 0;
  if ((h_us = ld_u16_be(x + 8)) > 9999) return -1;
  caltime_tai(&ct, &t->sec);
  t->nano = 100000 * h_us;
  t->atto = 0;
  return 0;
}

typedef struct {
  i64 sequence_number;
  u8 station_identifier[6];
  u8 location_identifier[3];
  u8 channel_identifier[4];
  u8 network_code[3];
  struct taia start_time;
  u16 num_samples;
  i16 sample_rate_factor;
  i16 sample_rate_multiplier;
  u8 activity_flags;
  u8 io_flags;
  u8 data_quality_flags;
  u8 blockette_count;
  i32 time_correction;
  u16 data_offset;
  u16 blockette_offset;
} data_record_header;

/* Reads a data record header and advances the files buffer accordingly. */
static int read_data_record_header(seedfile_t *f, data_record_header *h)
{
  u8 x[48];
  buffer_read(f->buffer, x, 48);
  /* Read sequence number. */
  parse_int(&h->sequence_number, x, 6);
  /* Read fixed fields. */
  byte_copy_0(h->station_identifier,  5, x +  8);
  byte_copy_0(h->location_identifier, 2, x + 13);
  byte_copy_0(h->channel_identifier,  3, x + 15);
  byte_copy_0(h->network_code,        2, x + 18);
  /* Read start time. */
  read_btime(&h->start_time, x + 20);
  /* Read samle rate and number. */
  h->num_samples            = ld_u16_be(x + 30);
  h->sample_rate_factor     = ld_i16_be(x + 32);
  h->sample_rate_multiplier = ld_i16_be(x + 34);
  h->activity_flags         = x[36];
  h->io_flags               = x[37];
  h->data_quality_flags     = x[38];
  h->blockette_count        = x[39];
  h->time_correction        = ld_i32_be(x + 40);
  h->data_offset            = ld_u16_be(x + 44);
  h->blockette_offset       = ld_u16_be(x + 46);
  return 0;
}

#define BUFFER_SIZE 4096

static void fill_buffer(seedfile_t *f)
{
  u8 data[BUFFER_SIZE];
  u64 s = buffer_space(f->buffer);
  i64 l = fread(data, 1, s, f->file_handle);
  if (l > 0) {
    buffer_write(f->buffer, data, l);
  } else if (l == 0) {
    f->eof = 1;
  }
}

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

static const u8 dr[] = "Data record";

int seed_begin_read(seedfile_t *file, seed_data_cb data_cb, seed_alloc_cb alloc_cb)
{
  data_record_header h;
  seed_buffer_t buf;
  int n;
  if (!file || file->mode != 0) return -1;
  fill_buffer(file);
  n = next_record(file);
  if (n == 'D') {
    read_data_record_header(file, &h);
    alloc_cb(file, sizeof(dr), &buf);
    byte_copy(buf.data, sizeof(dr), dr);
    data_cb(file, sizeof(dr), &buf);
  }
  return 0;
}
