#include "segy.h"

#define FOR(i, n) for (i = 0; i < n; ++i)

typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

/* Read 32-bit int from buffer. */
static i32 ld_i32_be(const u8 *x)
{
  i32 u = x[0];
  u = (u<<8)|x[1];
  u = (u<<8)|x[2];
  return (u<<8)|x[3];
}

/* Read 16-bit int from buffer. */
static i16 ld_i16_be(const u8 *x)
{
  i16 u = x[0];
  return (u<<8)|x[1];
}

/* Store 32-bit int to buffer. */
static void st_i32_be(u8 *x, i32 u)
{
  x[3] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[0] = u;
}

/* Store 16-bit int to buffer. */
static void st_i16_be(u8 *x, i16 u)
{
  x[1] = u; u >>= 8;
  x[0] = u;
}

static i64 mod(i64 a, i64 b)
{
  return ((a % b) + b) % b;
}

static void byte_copy(u8 *to, u64 l, const u8 *from)
{
  u64 i;
  FOR(i, l) to[i] = from[i];
}

static void byte_set(u8 *to, u64 l, u8 v)
{
  u64 i;
  FOR(i, l) to[i] = v;
}

int segy_text_header_read(segy_text_header_t *header, const uint8_t *buffer)
{
  if (!header || !buffer) return -1;
  byte_copy(header->content, 3200, buffer);
  return 3200;
}

int segy_text_header_write(uint8_t *buffer, segy_text_header_t *header)
{
  if (!header || !buffer) return -1;
  byte_copy(buffer, 3200, header->content);
  return 3200;
}

int segy_binary_header_read(segy_binary_header_t *header, const uint8_t *buffer)
{
  if (!header || !buffer) return -1;
  header->job_id                          = ld_i32_be(buffer);
  header->line_number                     = ld_i32_be(buffer + 4);
  header->reel_number                     = ld_i32_be(buffer + 8);
  header->data_traces_per_ensemble        = ld_i16_be(buffer + 12);
  header->auxiliary_traces_per_ensemble   = ld_i16_be(buffer + 14);
  header->sample_interval                 = ld_i16_be(buffer + 16);
  header->sample_interval_original        = ld_i16_be(buffer + 18);
  header->samples_per_data_trace          = ld_i16_be(buffer + 20);
  header->samples_per_data_trace_original = ld_i16_be(buffer + 22);
  header->format                          = ld_i16_be(buffer + 24);
  header->ensemble_fold                   = ld_i16_be(buffer + 26);
  header->trace_sorting                   = ld_i16_be(buffer + 28);
  header->vertical_sum                    = ld_i16_be(buffer + 30);
  header->sweep_frequency_start           = ld_i16_be(buffer + 32);
  header->sweep_frequency_end             = ld_i16_be(buffer + 34);
  header->sweep_length                    = ld_i16_be(buffer + 36);
  header->sweep_type                      = ld_i16_be(buffer + 38);
  header->sweep_channel_trace_number      = ld_i16_be(buffer + 40);
  header->sweep_trace_taper_length_start  = ld_i16_be(buffer + 42);
  header->sweep_trace_taper_length_end    = ld_i16_be(buffer + 44);
  header->taper_type                      = ld_i16_be(buffer + 46);
  /* 2 = YES, 1 = NO (Other values will be treated as NO.) */
  header->correlated_data_traces          = ld_i16_be(buffer + 48) == 2 ? SEGY_YES : SEGY_NO;
  /* 1 = YES, 2 = NO (I know, it's stupid ...) */
  header->binary_gain_recovered           = ld_i16_be(buffer + 50) == 1 ? SEGY_YES : SEGY_NO;
  header->amplitude_recovery              = ld_i16_be(buffer + 52);
  /* Using trace unit constants. */
  header->measurement_system              = ld_i16_be(buffer + 54) == 2 ? SEGY_FEET : SEGY_METER;
  header->signal_polarity                 = ld_i16_be(buffer + 56);
  /* Get approximate polarity. See below. */
  header->vibratory_polarity              = mod(ld_i16_be(buffer + 58) - 1, 8) * 45;
  header->fixed_trace_length              = ld_i16_be(buffer + 302);
  header->num_extended_textual_headers    = ld_i16_be(buffer + 304);
  return 400;
}

int segy_binary_header_write(uint8_t *buffer, segy_binary_header_t *header)
{
  if (!header || !buffer) return -1;
  st_i32_be(buffer,       header->job_id);
  st_i32_be(buffer +   4, header->line_number);
  st_i32_be(buffer +   8, header->reel_number);
  st_i16_be(buffer +  12, header->data_traces_per_ensemble);
  st_i16_be(buffer +  14, header->auxiliary_traces_per_ensemble);
  st_i16_be(buffer +  16, header->sample_interval);
  st_i16_be(buffer +  18, header->sample_interval_original);
  st_i16_be(buffer +  20, header->samples_per_data_trace);
  st_i16_be(buffer +  22, header->samples_per_data_trace_original);
  st_i16_be(buffer +  24, header->format);
  st_i16_be(buffer +  26, header->ensemble_fold);
  st_i16_be(buffer +  28, header->trace_sorting);
  st_i16_be(buffer +  30, header->vertical_sum);
  st_i16_be(buffer +  32, header->sweep_frequency_start);
  st_i16_be(buffer +  34, header->sweep_frequency_end);
  st_i16_be(buffer +  36, header->sweep_length);
  st_i16_be(buffer +  38, header->sweep_type);
  st_i16_be(buffer +  40, header->sweep_channel_trace_number);
  st_i16_be(buffer +  42, header->sweep_trace_taper_length_start);
  st_i16_be(buffer +  44, header->sweep_trace_taper_length_end);
  st_i16_be(buffer +  46, header->taper_type);
  /* 2 = YES, 1 = NO (Other values will be treated as NO.) */
  st_i16_be(buffer +  48, header->correlated_data_traces == SEGY_YES ? 2 : 1);
  /* 1 = YES, 2 = NO (I know, it's stupid ...) */
  st_i16_be(buffer +  50, header->binary_gain_recovered == SEGY_YES ? 1 : 2);
  st_i16_be(buffer +  52, header->amplitude_recovery);
  /* Using trace unit constants. */
  st_i16_be(buffer +  54, header->measurement_system == SEGY_FEET ? 2 : 1);
  st_i16_be(buffer +  56, header->signal_polarity);
  /* Set approximate polarity. Divides the full circle in 8 parts.
     338° -  22° -> 1
      23° -  67° -> 2
            ...
     293° - 337° -> 8 */
  st_i16_be(buffer +  58, mod(header->vibratory_polarity + 22, 360) / 45 + 1);
  /* Bytes 60 to 299 are unassigned. */
  byte_set(buffer + 60, 240, 0);
  /* Format revision */
  st_i16_be(buffer + 300, 0x0100);
  st_i16_be(buffer + 302, header->fixed_trace_length);
  st_i16_be(buffer + 304, header->num_extended_textual_headers);
  /* Bytes 306 to 399 are unassigned. */
  byte_set(buffer + 306, 94, 0);
  return 400;
}
