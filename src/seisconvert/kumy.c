#include "kumy.h"

#define FOR(i, n) for (i = 0; i < n; ++i)

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

/* Read 32-bit int from buffer. */
static u32 ld32(const u8 *x)
{
  u32 u = x[0];
  u = (u<<8)|x[1];
  u = (u<<8)|x[2];
  return (u<<8)|x[3];
}

/* Store 32-bit int to buffer. */
static void st32(u8 *x, u32 u)
{
  int i;
  for (i = 3; i >= 0; --i) { x[i] = u; u >>= 8; }
}

/* Store 16-bit int to buffer. */
static void st16(u8 *x, u32 u)
{
  int i;
  for (i = 1; i >= 0; --i) { x[i] = u; u >>= 8; }
}

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

int kumy_binary_header_read(kumy_binary_header_t *header, const uint8_t *buffer)
{
  int r = 0; /* Return code. Don't stop when there are inconsistencies, but tell when there were. */
  u32 j = ld32(buffer);
  header->job_id = j;
  header->year = (j >> 20) & 0xfff;       /* First 12 bit of job_id */
  header->julian_day = (j >> 11) & 0x1ff; /* Next 9 bit */
  header->recorder_no = j & 0x7ff;        /* Last 11 bit */
  j = ld32(buffer + 8);                   /* recorder_no * 100 + channel_no */
  if (j / 100 != header->recorder_no) r = -1;
  header->channel_no = j % 100;
  header->sample_interval = ld32(buffer + 16);
  header->num_samples = ld32(buffer + 20);
  return r;
}

int kumy_binary_header_write(u8 *buffer, kumy_binary_header_t *header)
{
  int i;
  FOR (i, 400) buffer[i] = 0;
  st32(buffer, header->job_id);
  st32(buffer + 8, header->recorder_no * 100 + header->channel_no);
  st16(buffer + 12, 1);
  st32(buffer + 16, header->sample_interval);
  st32(buffer + 20, header->num_samples);
  st16(buffer + 24, 2);
  st16(buffer + 26, 1);
  st16(buffer + 28, 1);
  st16(buffer + 30, 1);
  st16(buffer + 54, 1);
  st16(buffer + 56, 1);
  st16(buffer + 300, 0x100);
  return 0;
}

int kumy_text_header_read(kumy_text_header_t *header, const uint8_t *buffer)
{
  byte_copy(header->content, 3200, buffer);
  byte_copy_0(header->client, 22, buffer + 11);
  byte_copy_0(header->company, 22, buffer + 42);
  byte_copy_0(header->crew_no, 8, buffer + 72);
  byte_copy_0(header->line, 11, buffer + 89);
  byte_copy_0(header->area, 24, buffer + 105);
  byte_copy_0(header->reel_day_start, 3, buffer + 200);
  byte_copy_0(header->reel_year, 4, buffer + 209);
  byte_copy_0(header->observer, 17, buffer + 223);
  byte_copy_0(header->instrument, 15, buffer + 256);
  byte_copy_0(header->model, 11, buffer + 277);
  byte_copy_0(header->serial_no, 22, buffer + 298);
  return 0;
}

int kumy_text_header_write(uint8_t *buffer, kumy_text_header_t *header)
{
  int i;
  FOR (i, 3200) buffer[i] = 0;
  byte_copy(buffer, 3200, header->content);
  return 0;
}

kumy_frame_config_t kumy_get_frame_config(kumy_binary_header_t *header)
{
  return (4 << 24) | (2 << 16) | (32 << 8) | 1;
}

#define NUM_CHANNELS(fc) ((fc) & 255)
#define BIT_DEPTH(fc) (((fc) >> 8) & 255)
#define FORMAT(fc) (((fc) >> 16) & 255)
#define FRAME_SIZE(fc) (((fc) >> 24) & 255)

int kumy_get_frame_size(kumy_frame_config_t fc)
{
  return FRAME_SIZE(fc);
}

int kumy_read_int_frame(kumy_frame_config_t fc, int32_t *samples, const uint8_t *buffer)
{
  int i;
  FOR(i, NUM_CHANNELS(fc))
    samples[0] = (int32_t)ld32(buffer);
  return FRAME_SIZE(fc);
}

int kumy_write_int_frame(kumy_frame_config_t fc, uint8_t *buffer, const int32_t *samples)
{
  int i;
  FOR(i, NUM_CHANNELS(fc))
    st32(buffer, (uint32_t)samples[0]);
  return FRAME_SIZE(fc);
}

int kumy_read_double_frame(kumy_frame_config_t fc, double *samples, const uint8_t *buffer)
{
  int i;
  double s = 1.0 / 0x7fffffff;
  FOR(i, NUM_CHANNELS(fc)) {
    samples[i] = (int32_t)ld32(buffer) * s;
    buffer += 4;
  }
  return FRAME_SIZE(fc);
}

int kumy_write_double_frame(kumy_frame_config_t fc, uint8_t *buffer, const double *samples)
{
  int i;
  double s = 0x7fffffff;
  FOR(i, NUM_CHANNELS(fc)) {
    st32(buffer, (int32_t)(samples[i] * s));
    buffer += 4;
  }
  return FRAME_SIZE(fc);
}
