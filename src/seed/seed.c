#include "seed.h"

#include <stdlib.h>

#include "number.h"
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

/* Copy l bytes from `from` to `to`. */
static void byte_copy(u8 *to, u64 l, const u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
}

/* Copy l bytes from `from` to `to`. Appends 0 byte to `to`. */
static void byte_copy_0(u8 *to, u64 l, const u8 *from)
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
static int next_record(const u8 *x)
{
  i64 no;
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

/* Reads a data record header. */
int read_data_record_header(data_record_header *h, const u8 *x)
{
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

int read_blockette_1000(blockette_1000 *b, const u8 *x)
{
  if (ld_u16_be(x) != 1000) return -3;
  b->next_blockette = ld_u16_be(x + 2);
  b->encoding = x[4];
  b->word_order = x[5];
  b->data_record_length = x[6];
  return 0;
}

#define BUFFER_SIZE 4096

seedfile_t *seed_open(const char *path)
{
  /* Allocate memory for the handle. */
  seedfile_t *file = malloc(sizeof(seedfile_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "r"))) goto err1;
  file->mode = 0;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
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
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err1:
  free(file);
  return 0;
}

void seed_close(seedfile_t *file)
{
  if (!file) return;
  fclose(file->file_handle);
  free(file);
}

int seed_begin_read(seedfile_t *file, seed_data_cb data_cb, seed_alloc_cb alloc_cb)
{
  data_record_header h;
  blockette_1000 _1000;
  seed_buffer_t buf;
  int n, eof = 0;
  i64 l;
  u64 s;
  u8 b[128];
  if (!file || file->mode != 0) return -1;
  while (!eof) {
    l = fread(b, 1, 128, file->file_handle);
    if (l > 0) {
      n = next_record(b);
      if (n == 'D') {
        if (read_data_record_header(&h, b) < 0) return -1;
        if (read_blockette_1000(&_1000, b + h.blockette_offset) < 0) return -1;
        s = 1 << _1000.data_record_length;
        alloc_cb(file, s, &buf);
        if (s > 128) {
          byte_copy(buf.data, 128, b);
          l = fread(b, 1, s - 128, file->file_handle);
        } else {
          byte_copy(buf.data, s, b);
        }
        data_cb(file, s, &buf);
      } else {
        return -1;
      }
    } else {
      eof = 1;
    }
  }
  return 0;
}
