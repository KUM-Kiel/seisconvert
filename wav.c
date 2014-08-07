#include "wav.h"

#define FOR(i, n) for (i = 0; i < n; ++i)

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

/* Read 32-bit int from buffer. */
static u32 ld32(const u8 *x)
{
  u32 u = x[3];
  u = (u<<8)|x[2];
  u = (u<<8)|x[1];
  return (u<<8)|x[0];
}

/* Read 16-bit int from buffer. */
static u32 ld16(const u8 *x)
{
  u32 u = x[1];
  return (u<<8)|x[0];
}

/* Store 32-bit int to buffer. */
static void st32(u8 *x, u32 u)
{
  int i;
  FOR(i, 4) { x[i] = u; u >>= 8; }
}

/* Store 16-bit int to buffer. */
static void st16(u8 *x, u32 u)
{
  int i;
  FOR(i, 2) { x[i] = u; u >>= 8; }
}

static void byte_copy(u8 *to, u64 l, u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
}

static int byte_equal(const u8 *a, u64 l, const u8 *b)
{
  u64 i;
  FOR(i, l) {
    if (a[i] != b[i])
      return 0;
  }
  return 1;
}

/* WAV header
typedef struct {
  uint8_t  riff_hdr[4];
  uint32_t file_size;
  uint8_t  wave_hdr[4];
  uint8_t  fmt_hdr[4];
  uint32_t fmt_size;
  uint16_t format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t frame_size;
  uint16_t bit_depth;
  uint8_t  data_hdr[4];
  uint32_t data_size;
} wav_header; */

int wav_header_read(wav_header_t *header, uint8_t *buffer)
{
  u32 num_channels, frame_size, bit_depth, sample_rate, file_size, data_size;
  if (!header || !buffer) return -1;
  num_channels = ld16(buffer + 22);
  frame_size = ld16(buffer + 32);
  bit_depth = ld16(buffer + 34);
  sample_rate = ld32(buffer + 24);
  file_size = ld32(buffer + 4);
  data_size = ld32(buffer + 40);
  if (
    /* Constant fields */
    !byte_equal(buffer,      4, (u8*)"RIFF") ||
    !byte_equal(buffer +  8, 4, (u8*)"WAVE") ||
    !byte_equal(buffer + 12, 4, (u8*)"fmt ") ||
    !byte_equal(buffer + 36, 4, (u8*)"data") ||
    /* Frame size is standardised. */
    frame_size != num_channels * ((bit_depth + 7) / 8) ||
    /* File must be large enough to contain all the data. */
    data_size + 36 > file_size ||
    /* Byterate is redundant. */
    ld32(buffer + 28) != frame_size * sample_rate
  ) return -1;
  header->format = ld16(buffer + 20);
  header->num_channels = num_channels;
  header->sample_rate = sample_rate;
  header->bit_depth = bit_depth;
  header->num_frames = data_size / frame_size;
  return 44;
}

int wav_header_write(uint8_t *buffer, wav_header_t *header)
{
  u32 frame_size, data_size;
  if (!header || !buffer) return -1;
  frame_size = header->num_channels * ((header->bit_depth + 7) / 8);
  data_size = header->num_frames * frame_size;
  byte_copy(buffer,      4, (u8*)"RIFF");
  st32(buffer +  4, data_size + 36);
  byte_copy(buffer +  8, 4, (u8*)"WAVE");
  byte_copy(buffer + 12, 4, (u8*)"fmt ");
  st32(buffer + 16, 16);
  st16(buffer + 20, header->format);
  st16(buffer + 22, header->num_channels);
  st32(buffer + 24, header->sample_rate);
  st32(buffer + 28, frame_size * header->sample_rate);
  st16(buffer + 32, frame_size);
  st16(buffer + 34, header->bit_depth);
  byte_copy(buffer + 36, 4, (u8*)"data");
  st32(buffer + 40, data_size);
  return 44;
}

wav_frame_config_t wav_get_frame_config(wav_header_t *header)
{
  wav_frame_config_t fc = 0;
  fc |= (header->num_channels & 255);
  fc |= (header->bit_depth & 255) << 8;
  fc |= (header->format & 255) << 16;
  fc |= ((header->num_channels * ((header->bit_depth + 7) / 8)) & 255) << 24;
  return fc;
}

#define NUM_CHANNELS(fc) ((fc) & 255)
#define BIT_DEPTH(fc) (((fc) >> 8) & 255)
#define FORMAT(fc) (((fc) >> 16) & 255)
#define FRAME_SIZE(fc) (((fc) >> 24) & 255)

int wav_get_frame_size(wav_frame_config_t fc)
{
  return FRAME_SIZE(fc);
}

int wav_read_int_frame(wav_frame_config_t fc, int32_t *samples, const uint8_t *buffer)
{
  int i, b;
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        samples[i] = (int32_t)buffer[i] << 24;
        break;
      case 12:
      case 16:
        samples[i] = (int32_t)ld16(buffer) << 16;
        buffer += 2;
        break;
      case 32:
        samples[i] = (int32_t)ld32(buffer);
        buffer += 4;
        break;
      default:
        return -1;
    }
  }
  return FRAME_SIZE(fc);
}

int wav_write_int_frame(wav_frame_config_t fc, uint8_t *buffer, const int32_t *samples)
{
  int i, b;
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        buffer[i] = (uint8_t)(samples[i] >> 24);
        break;
      case 12:
      case 16:
        st16(buffer, (uint32_t)(samples[i] >> 16));
        buffer += 2;
        break;
      case 32:
        st32(buffer, (uint32_t)samples[i]);
        buffer += 4;
        break;
      default:
        return -1;
    }
  }
  return FRAME_SIZE(fc);
}

int wav_read_double_frame(wav_frame_config_t fc, double *samples, const uint8_t *buffer)
{
  int i, b;
  double s = 0;
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  switch (b) {
    case 8:  s = 1.0 / 0x7f; break;
    case 12: s = 1.0 / 0x7ff0; break;
    case 16: s = 1.0 / 0x7fff; break;
    case 32: s = 1.0 / 0x7fffffff; break;
  }
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        samples[i] = (int8_t)buffer[i] * s;
        break;
      case 12:
      case 16:
        samples[i] = (int16_t)ld16(buffer) * s;
        buffer += 2;
        break;
      case 32:
        samples[i] = (int32_t)ld32(buffer) * s;
        buffer += 4;
        break;
      default:
        return -1;
    }
  }
  return FRAME_SIZE(fc);
}

int wav_write_double_frame(wav_frame_config_t fc, uint8_t *buffer, const double *samples)
{
  int i, b;
  double s = 0;
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  switch (b) {
    case 8:  s = 0x7f; break;
    case 12: s = 0x7ff0; break;
    case 16: s = 0x7fff; break;
    case 32: s = 0x7fffffff; break;
  }
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        buffer[i] = (int8_t)(samples[i] * s);
        break;
      case 12:
      case 16:
        st16(buffer, (int32_t)(samples[i] * s));
        buffer += 2;
        break;
      case 32:
        st32(buffer, (int32_t)(samples[i] * s));
        buffer += 4;
        break;
      default:
        return -1;
    }
  }
  return FRAME_SIZE(fc);
}
