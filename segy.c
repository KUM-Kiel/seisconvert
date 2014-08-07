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
static i32 ld32(const u8 *x)
{
  i32 u = x[0];
  u = (u<<8)|x[1];
  u = (u<<8)|x[2];
  return (u<<8)|x[3];
}

/* Read 16-bit int from buffer. */
static i16 ld16(const u8 *x)
{
  i16 u = x[0];
  return (u<<8)|x[1];
}

/* Store 32-bit int to buffer. */
static void st32(u8 *x, i32 u)
{
  x[3] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[0] = u;
}

/* Store 16-bit int to buffer. */
static void st16(u8 *x, i16 u)
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
  header->job_id                          = ld32(buffer);
  header->line_number                     = ld32(buffer + 4);
  header->reel_number                     = ld32(buffer + 8);
  header->data_traces_per_ensemble        = ld16(buffer + 12);
  header->auxiliary_traces_per_ensemble   = ld16(buffer + 14);
  header->sample_interval                 = ld16(buffer + 16);
  header->sample_interval_original        = ld16(buffer + 18);
  header->samples_per_data_trace          = ld16(buffer + 20);
  header->samples_per_data_trace_original = ld16(buffer + 22);
  header->format                          = ld16(buffer + 24);
  header->ensemble_fold                   = ld16(buffer + 26);
  header->trace_sorting                   = ld16(buffer + 28);
  header->vertical_sum                    = ld16(buffer + 30);
  header->sweep_frequency_start           = ld16(buffer + 32);
  header->sweep_frequency_end             = ld16(buffer + 34);
  header->sweep_length                    = ld16(buffer + 36);
  header->sweep_type                      = ld16(buffer + 38);
  header->sweep_channel_trace_number      = ld16(buffer + 40);
  header->sweep_trace_taper_length_start  = ld16(buffer + 42);
  header->sweep_trace_taper_length_end    = ld16(buffer + 44);
  header->taper_type                      = ld16(buffer + 46);
  /* 2 = YES, 1 = NO (Other values will be treated as NO.) */
  header->correlated_data_traces          = ld16(buffer + 48) == 2 ? SEGY_YES : SEGY_NO;
  /* 1 = YES, 2 = NO (I know, it's stupid ...) */
  header->binary_gain_recovered           = ld16(buffer + 50) == 1 ? SEGY_YES : SEGY_NO;
  header->amplitude_recovery              = ld16(buffer + 52);
  header->measurement_system              = ld16(buffer + 54);
  header->signal_polarity                 = ld16(buffer + 56);
  /* Get approximate polarity. See below. */
  header->vibratory_polarity              = mod(ld16(buffer + 58) - 1, 8) * 45;
  header->fixed_trace_length              = ld16(buffer + 302);
  header->num_extended_textual_headers    = ld16(buffer + 304);
  return 400;
}

int segy_binary_header_write(uint8_t *buffer, segy_binary_header_t *header)
{
  if (!header || !buffer) return -1;
  st32(buffer,       header->job_id);
  st32(buffer +   4, header->line_number);
  st32(buffer +   8, header->reel_number);
  st16(buffer +  12, header->data_traces_per_ensemble);
  st16(buffer +  14, header->auxiliary_traces_per_ensemble);
  st16(buffer +  16, header->sample_interval);
  st16(buffer +  18, header->sample_interval_original);
  st16(buffer +  20, header->samples_per_data_trace);
  st16(buffer +  22, header->samples_per_data_trace_original);
  st16(buffer +  24, header->format);
  st16(buffer +  26, header->ensemble_fold);
  st16(buffer +  28, header->trace_sorting);
  st16(buffer +  30, header->vertical_sum);
  st16(buffer +  32, header->sweep_frequency_start);
  st16(buffer +  34, header->sweep_frequency_end);
  st16(buffer +  36, header->sweep_length);
  st16(buffer +  38, header->sweep_type);
  st16(buffer +  40, header->sweep_channel_trace_number);
  st16(buffer +  42, header->sweep_trace_taper_length_start);
  st16(buffer +  44, header->sweep_trace_taper_length_end);
  st16(buffer +  46, header->taper_type);
  /* 2 = YES, 1 = NO (Other values will be treated as NO.) */
  st16(buffer +  48, header->correlated_data_traces == SEGY_YES ? 2 : 1);
  /* 1 = YES, 2 = NO (I know, it's stupid ...) */
  st16(buffer +  50, header->binary_gain_recovered == SEGY_YES ? 1 : 2);
  st16(buffer +  52, header->amplitude_recovery);
  st16(buffer +  54, header->measurement_system);
  st16(buffer +  56, header->signal_polarity);
  /* Set approximate polarity. Divides the full circle in 8 parts.
     338° -  22° -> 1
      23° -  67° -> 2
            ...
     293° - 337° -> 8 */
  st16(buffer +  58, mod(header->vibratory_polarity + 22, 360) / 45 + 1);
  /* Bytes 60 to 299 are unassigned. */
  byte_set(buffer + 60, 240, 0);
  /* Format revision */
  st16(buffer + 300, 0x0100);
  st16(buffer + 302, header->fixed_trace_length);
  st16(buffer + 304, header->num_extended_textual_headers);
  /* Bytes 306 to 399 are unassigned. */
  byte_set(buffer + 306, 94, 0);
  return 400;
}
