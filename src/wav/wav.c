#include "wav.h"
#include "number.h"

/* Shortcut for for loops in standard form. */
#define FOR(i, n) for (i = 0; i < n; ++i)

/* Shortcuts for integer types. */
typedef  uint8_t u8;
typedef   int8_t i8;
typedef uint16_t u16;
typedef  int16_t i16;
typedef uint32_t u32;
typedef  int32_t i32;
typedef uint64_t u64;

/* Copies l bytes from from to to. */
static void byte_copy(u8 *to, u64 l, const u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
}

/* Checks l bytes for equality.
 * Caveat: The function returns as soon as it detects a difference and
 * does not necessarily check all bytes. So it is vulnurable to timing
 * attacks. */
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
 *  0 -  4: "RIFF"
 *  4 -  8: File size - 8
 *  8 - 12: "WAVE"
 * 12 - 16: "fmt "
 * 16 - 20: Format header size (always 16)
 * 20 - 22: Format (1 for PCM)
 * 22 - 24: Number of channels
 * 24 - 28: Sample rate
 * 28 - 32: Byte rate
 * 32 - 34: Frame size
 * 34 - 36: Bit depth
 * 36 - 40: "data"
 * 40 - 44: Size of data chunk */

/* Function for reading WAV headers from a buffer.
 * First loads fields required for redundancy checks, then does the check.
 * In case of a failure it returns -1 without touching the supplied header.
 * Lastly it fills in the header. */
int wav_header_read(wav_header_t *header, const u8 *buffer)
{
  u32 num_channels, frame_size, bit_depth, sample_rate, file_size, data_size, format;
  /* Check for valid pointers. */
  if (!header || !buffer) return -1;
  format = ld_u16_le(buffer + 20);
  num_channels = ld_u16_le(buffer + 22);
  frame_size = ld_u16_le(buffer + 32);
  bit_depth = ld_u16_le(buffer + 34);
  sample_rate = ld_u32_le(buffer + 24);
  file_size = ld_u32_le(buffer + 4);
  data_size = ld_u32_le(buffer + 40);
  if (
    /* Constant fields */
    !byte_equal(buffer,      4, (u8*)"RIFF") ||
    !byte_equal(buffer +  8, 4, (u8*)"WAVE") ||
    !byte_equal(buffer + 12, 4, (u8*)"fmt ") ||
    !byte_equal(buffer + 36, 4, (u8*)"data") ||
    /* Format header is always 16 bytes. */
    ld_u32_le(buffer + 16) != 16 ||
    /* Only PCM is supported. */
    format != WAV_PCM ||
    /* Only 8, 12, 16, 24 and 32 bit depth are supported. */
    (
      bit_depth != 8 &&
      bit_depth != 12 &&
      bit_depth != 16 &&
      bit_depth != 24 &&
      bit_depth != 32
    ) ||
    /* There must be at least one channel. */
    num_channels == 0 ||
    /* frame_size is redundant. It can be calculated from bit_depth and num_channels. */
    frame_size != num_channels * ((bit_depth + 7) / 8) ||
    /* File must be large enough to contain all the data. */
    data_size + 36 > file_size ||
    /* byte_rate is redundant. It can be calculated from frame_size and sample_rate. */
    ld_u32_le(buffer + 28) != frame_size * sample_rate
  ) return -1;
  /* Fill in header. */
  header->format = format;
  header->num_channels = num_channels;
  header->sample_rate = sample_rate;
  header->bit_depth = bit_depth;
  header->num_frames = data_size / frame_size;
  /* Return bytes read. */
  return WAV_HEADER_BYTES;
}

/* Function for writing WAV headers to a buffer.
 * First calculates the redundant fields, which are not present in the header
 * structure. Then stores all the fields along with the constant values to the
 * buffer and returns the number of bytes stored.
 * Only fails if zero pointers are supplied or format or bit_depth are
 * unsupported. The header structure is not checked further as any values are
 * allowed. */
int wav_header_write(u8 *buffer, wav_header_t *header)
{
  u32 frame_size, data_size;
  /* Check for valid pointers. */
  if (!header || !buffer) return -1;
  /* Check for valid format and bit_depth. */
  if (
    header->format != WAV_PCM ||
    /* Only 8, 12, 16, 24 and 32 bit depth are supported. */
    (
      header->bit_depth != 8 &&
      header->bit_depth != 12 &&
      header->bit_depth != 16 &&
      header->bit_depth != 24 &&
      header->bit_depth != 32
    ) ||
    /* There must be at least one channel. */
    header->num_channels == 0
  ) return -1;
  /* Calculate redundant fields. */
  frame_size = header->num_channels * ((header->bit_depth + 7) / 8);
  data_size = header->num_frames * frame_size;
  /* Fill in the fields. */
  byte_copy(buffer,      4, (u8*)"RIFF");
  st_u32_le(buffer +  4, data_size + 36);
  byte_copy(buffer +  8, 4, (u8*)"WAVE");
  byte_copy(buffer + 12, 4, (u8*)"fmt ");
  st_u32_le(buffer + 16, 16);
  st_u16_le(buffer + 20, header->format);
  st_u16_le(buffer + 22, header->num_channels);
  st_u32_le(buffer + 24, header->sample_rate);
  st_u32_le(buffer + 28, frame_size * header->sample_rate);
  st_u16_le(buffer + 32, frame_size);
  st_u16_le(buffer + 34, header->bit_depth);
  byte_copy(buffer + 36, 4, (u8*)"data");
  st_u32_le(buffer + 40, data_size);
  /* Return bytes written. */
  return WAV_HEADER_BYTES;
}

/* Return frame config. It's contents are:
 * 2 byte frame_size,
 * 2 byte format,
 * 2 byte bit_depth,
 * 2 byte num_channels. */
wav_frame_config_t wav_get_frame_config(wav_header_t *header)
{
  wav_frame_config_t fc = 0;
  /* Check for valid pointer. */
  if (!header) return 0;
  /* Fill in bytes. */
  fc |= (wav_frame_config_t)(header->num_channels & 0xffff);
  fc |= (wav_frame_config_t)(header->bit_depth & 0xffff) << 16;
  fc |= (wav_frame_config_t)(header->format & 0xffff) << 32;
  fc |= (wav_frame_config_t)((header->num_channels * ((header->bit_depth + 7) / 8)) & 0xffff) << 48;
  return fc;
}

/* Macros for accessing the fields of frame configs. */
#define NUM_CHANNELS(fc) ((fc) & 0xffff)
#define BIT_DEPTH(fc) (((fc) >> 16) & 0xffff)
#define FORMAT(fc) (((fc) >> 32) & 0xffff)
#define FRAME_SIZE(fc) (((fc) >> 48) & 0xffff)

/* Exported functions for accessing the fields of frame configs. */
int wav_get_frame_size(wav_frame_config_t fc)
{
  return FRAME_SIZE(fc);
}

int wav_get_num_channels(wav_frame_config_t fc)
{
  return NUM_CHANNELS(fc);
}

/* Helper function for reading samples with 24 bit depth.
 * Returns an i32 with the least significant byte set to zero! */
static i32 read_sample_24(const u8 *b)
{
  return (i32)b[0] << 8 | (i32)b[1] << 16 | (i32)b[2] << 24;
}

/* Helper function for writing samples with 24 bit depth.
 * Drops the least significant byte! */
static void write_sample_24(u8 *b, i32 s)
{
  b[0] = s >> 8;
  b[1] = s >> 16;
  b[2] = s >> 24;
}

/* Function for reading a frame from a buffer.
 * The frame is an i32 array with one field for each channel. */
int wav_read_int_frame(wav_frame_config_t fc, i32 *samples, const u8 *buffer)
{
  int i, b;
  /* Check for valid pointers. */
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  /* Read one sample for each channel. */
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        /* Just read a byte as signed 8 bit int.
         * This is the most significant byte, so it is shifted up. */
        samples[i] = (i32)buffer[i] << 24;
        break;
      case 12:
      case 16:
        /* 12 and 16 bit cases are identical. For 12 bit depth normally the
         * least significant 4 bits are zero, but we will not complain if they
         * are not.
         * Again, the bytes read are the most significant ones, so we have to
         * shift up. */
        samples[i] = (i32)ld_i16_le(buffer) << 16;
        buffer += 2;
        break;
      case 24:
        /* For 24 bit use our helper function.
         * Here the bytes are already shifted. */
        samples[i] = read_sample_24(buffer);
        buffer += 3;
        break;
      case 32:
        /* For 32 bit the value is stored unchanged. */
        samples[i] = ld_i32_le(buffer);
        buffer += 4;
        break;
      default:
        /* Issue an error, if the bit depth is not supported. */
        return -1;
    }
  }
  /* Return bytes read. */
  return FRAME_SIZE(fc);
}

int wav_write_int_frame(wav_frame_config_t fc, u8 *buffer, const i32 *samples)
{
  int i, b;
  if (!samples || !buffer) return -1;
  /* Check for valid pointers. */
  b = BIT_DEPTH(fc);
  /* Write one sample for each channel. */
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        /* Only store the most significant byte. */
        buffer[i] = (u8)(samples[i] >> 24);
        break;
      case 12:
        /* For 12 bit we only take the most significant 12 bits.
         * So after the shift the least significant 4 bits should be zero. */
        st_i16_le(buffer, (samples[i] >> 16) & 0xfff0);
        buffer += 2;
        break;
      case 16:
        /* For 16 bit we keep the most significant two bytes. */
        st_i16_le(buffer, (samples[i] >> 16));
        buffer += 2;
        break;
      case 24:
        /* For 24 bit we use the helper function, which already does the shifting. */
        write_sample_24(buffer, samples[i]);
        buffer += 3;
        break;
      case 32:
        /* For 32 bit the value is again stored unchanged. */
        st_i32_le(buffer, samples[i]);
        buffer += 4;
        break;
      default:
        /* Issue an error, if the bit depth is not supported. */
        return -1;
    }
  }
  /* Return bytes written. */
  return FRAME_SIZE(fc);
}

int wav_read_double_frame(wav_frame_config_t fc, double *samples, const u8 *buffer)
{
  int i, b;
  double s = 0;
  /* Check for valid pointers. */
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  /* Calculate the scaling factor.
   * This determines which value should become 1.0. */
  switch (b) {
    case 8:  s = 1.0 / 0x7f; break;
    case 12: s = 1.0 / 0x7ff0; break;
    case 16: s = 1.0 / 0x7fff; break;
    case 24: s = 1.0 / 0x7fffff00; break;
    case 32: s = 1.0 / 0x7fffffff; break;
  }
  /* Read one sample for each channel. */
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        /* Read one byte signed and scale it.
         * This must be done with the correct cast for the sign to be
         * interpreted correctly. */
        samples[i] = (i8)buffer[i] * s;
        break;
      case 12:
      case 16:
        /* Read two bytes. Again the cast is important. */
        samples[i] = ld_i16_le(buffer) * s;
        buffer += 2;
        break;
      case 24:
        /* Read three bytes.
         * Here the cast is already done by the helper function. */
        samples[i] = read_sample_24(buffer) * s;
        buffer += 3;
        break;
      case 32:
        /* Read four bytes. Again the cast is important. */
        samples[i] = ld_i32_le(buffer) * s;
        buffer += 4;
        break;
      default:
        /* Issue an error, if the bit depth is not supported. */
        return -1;
    }
  }
  /* Return bytes read. */
  return FRAME_SIZE(fc);
}

/* Limits a to between -1 and 1.
 * We use this function to improve the overdriving behavior to clipping instead
 * of wrapping around. */
static double limit(double a)
{
  return a < -1 ? -1 : (a > 1 ? 1 : a);
}

int wav_write_double_frame(wav_frame_config_t fc, u8 *buffer, const double *samples)
{
  int i, b;
  double s = 0;
  /* Check for valid pointers. */
  if (!samples || !buffer) return -1;
  b = BIT_DEPTH(fc);
  /* Calculate the scaling factor.
   * This inverts the scaling above. */
  switch (b) {
    case 8:  s = 0x7f; break;
    case 12: s = 0x7ff0; break;
    case 16: s = 0x7fff; break;
    case 24: s = 0x7fffff00; break;
    case 32: s = 0x7fffffff; break;
  }
  /* Write one sample for each channel. */
  FOR(i, NUM_CHANNELS(fc)) {
    switch (b) {
      case 8:
        /* Store a scaled byte. */
        buffer[i] = (i8)(limit(samples[i]) * s);
        break;
      case 12:
        /* Store two bytes with the least significant 4 bits cleared. */
        st_i16_le(buffer, (i16)(limit(samples[i]) * s) & 0xfff0);
        buffer += 2;
        break;
      case 16:
        /* Store two bytes. */
        st_i16_le(buffer, (limit(samples[i]) * s));
        buffer += 2;
        break;
      case 24:
        /* Store three bytes with the helper function. */
        write_sample_24(buffer, limit(samples[i]) * s);
        buffer += 3;
        break;
      case 32:
        /* Store four bytes. */
        st_i32_le(buffer, (limit(samples[i]) * s));
        buffer += 4;
        break;
      default:
        /* Issue an error, if the bit depth is not supported. */
        return -1;
    }
  }
  /* Return bytes written. */
  return FRAME_SIZE(fc);
}
