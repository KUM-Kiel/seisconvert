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

/* Return the next record type. The buffer x must be positioned at the
 * beginning of a record. Returns -3 if the buffer is not at a record border.
 * Returns 'D', 'R', 'Q' or 'M' for a data record, 'V' for a volume header,
 * 'A' for a dictionary header, 'S' for a station header and 'T' for a time span
 * header. Returns a lowercase letter if the record continues the last one. */
int seed_record_type(const u8 *x)
{
  i64 no;
  if (parse_int(&no, x, 6) < 0) return -3;
  if (no < 0) return -3;
  if (x[7] == ' ') {
    switch (x[6]) {
      case 'D': return 'D';
      case 'R': return 'R';
      case 'Q': return 'Q';
      case 'M': return 'M';
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

/* Reads a data record header. */
int seed_read_data_record_header(seed_data_record_header_t *h, const u8 *x)
{
  seed_btime_t btime;
  /* Read sequence number. */
  parse_int(&h->sequence_number, x, 6);
  /* Read fixed fields. */
  byte_copy_0(h->station_identifier,  5, x +  8);
  byte_copy_0(h->location_identifier, 2, x + 13);
  byte_copy_0(h->channel_identifier,  3, x + 15);
  byte_copy_0(h->network_code,        2, x + 18);
  /* Read start time. */
  seed_read_btime(&btime, x + 20);
  seed_btime2taia(&h->start_time, &btime);
  /* Read stuff. */
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

int seed_write_data_record_header(u8 *x, const seed_data_record_header_t *h)
{
  seed_btime_t btime;
  /* Write sequence number. */
  write_int(x, 6, h->sequence_number, 1);
  byte_copy(x + 6, 2, (u8*)"D "); /* XXX: Support for other letters. */
  /* Write fixed fields. */
  byte_copy(x +  8, 5, h->station_identifier);
  byte_copy(x + 13, 2, h->location_identifier);
  byte_copy(x + 15, 3, h->channel_identifier);
  byte_copy(x + 18, 2, h->network_code);
  /* Write start time. */
  seed_taia2btime(&btime, &h->start_time);
  seed_write_btime(x + 20, &btime);
  /* Write stuff. */
  st_u16_be(x + 30, h->num_samples);
  st_i16_be(x + 32, h->sample_rate_factor);
  st_i16_be(x + 34, h->sample_rate_multiplier);
  x[36] = h->activity_flags;
  x[37] = h->io_flags;
  x[38] = h->data_quality_flags;
  x[39] = h->blockette_count;
  st_i32_be(x + 40, h->time_correction);
  st_u16_be(x + 44, h->data_offset);
  st_u16_be(x + 46, h->blockette_offset);
  return 0;
}

int seed_read_blockette_1000(seed_blockette_1000_t *b, const u8 *x)
{
  if (ld_u16_be(x) != 1000) return -3;
  b->next_blockette = ld_u16_be(x + 2);
  b->encoding = x[4];
  b->word_order = x[5];
  b->data_record_length = x[6];
  return 0;
}

int seed_write_blockette_1000(u8 *x, const seed_blockette_1000_t *b)
{
  st_u16_be(x, 1000);
  st_u16_be(x + 2, b->next_blockette);
  x[4] = b->encoding;
  x[5] = b->word_order;
  x[6] = b->data_record_length;
  x[7] = 0;
  return 0;
}

int seed_read_btime(seed_btime_t *btime, const uint8_t *buffer)
{
  btime->year       = ld_u16_be(buffer);
  btime->julian_day = ld_u16_be(buffer + 2);
  btime->hour       = buffer[4];
  btime->minute     = buffer[5];
  btime->second     = buffer[6];
  btime->tenth_ms   = ld_u16_be(buffer + 8);
  return 0;
}

int seed_write_btime(uint8_t *buffer, const seed_btime_t *btime)
{
  st_u16_be(buffer,     btime->year);
  st_u16_be(buffer + 2, btime->julian_day);
  buffer[4] = btime->hour;
  buffer[5] = btime->minute;
  buffer[6] = btime->second;
  buffer[7] = 0;
  st_u16_be(buffer + 8, btime->tenth_ms);
  return 0;
}

int seed_btime2taia(struct taia *t, const seed_btime_t *btime)
{
  struct caltime ct;
  ct.date.year = btime->year;
  ct.date.month = 1;
  ct.date.day = btime->julian_day;
  ct.hour = btime->hour;
  ct.minute = btime->minute;
  ct.second = btime->second;
  ct.offset = 0;
  caltime_tai(&ct, &t->sec);
  t->nano = 100000 * btime->tenth_ms;
  t->atto = 0;
  return 0;
}

int seed_taia2btime(seed_btime_t *btime, const struct taia *t)
{
  struct caltime ct;
  caltime_utc(&ct, &t->sec, 0, 0);

  btime->year = ct.date.year;
  btime->julian_day = julian_day(ct.date.day, ct.date.month, ct.date.year);
  btime->hour = ct.hour;
  btime->minute = ct.minute;
  btime->second = ct.second;
  btime->tenth_ms = t->nano / 100000;
  return 0;
}

double seed_sample_rate(int16_t factor, int16_t multiplier)
{
  if (factor >= 0 && multiplier >= 0) {
    return (double)factor * multiplier;
  } else if (factor >= 0 && multiplier < 0) {
    return -1.0 * factor / multiplier;
  } else if (factor < 0 && multiplier >= 0) {
    return -1.0 / factor * multiplier;
  } else {
    return 1.0 / factor / multiplier;
  }
}

int seed_sample_rate_from_int(uint32_t sample_rate, int16_t *factor, int16_t *multiplier)
{
  /* XXX: If samplerate is slightly below 1GHz, the result might be wrong. */
  uint32_t m = 1;
  if (sample_rate < 1073676289) { /* (2^15-1)^2 */
    while (sample_rate >= 32768) { /* 2^15 */
      sample_rate /= 10;
      m *= 10;
    }
    *factor = sample_rate;
    *multiplier = m;
  } else {
    *factor = 32767;
    *multiplier = 32767;
  }
  return 0;
}
