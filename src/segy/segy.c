#include "segy.h"
#include "number.h"

#define FOR(i, n) for (i = 0; i < n; ++i)

typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

/* Calculates the mathematical modulo operation.
 * The result has the same sign as b. */
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
   * 338° -  22° -> 1
   *  23° -  67° -> 2
   *        ...
   * 293° - 337° -> 8 */
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

int segy_trace_header_read(segy_trace_header_t *header, const uint8_t *buffer)
{
  int16_t u;
  if (!header || !buffer) return -1;
  header->sequence_number_in_line                 = ld_i32_be(buffer);
  header->sequence_number_in_file                 = ld_i32_be(buffer + 4);
  header->original_field_recor_number             = ld_i32_be(buffer + 8);
  header->original_trace_number                   = ld_i32_be(buffer + 12);
  header->energy_source_point_number              = ld_i32_be(buffer + 16);
  header->ensemble_number                         = ld_i32_be(buffer + 20);
  header->trace_number_in_ensemble                = ld_i32_be(buffer + 24);
  header->trace_identification                    = ld_i16_be(buffer + 28);
  header->number_of_vertically_summed_traces      = ld_i16_be(buffer + 30);
  header->number_of_horizontally_stacked_traces   = ld_i16_be(buffer + 32);
  header->data_use                                = ld_i16_be(buffer + 34);
  header->distance_from_source_to_receiver_group  = ld_i32_be(buffer + 36);
  header->receiver_group_elevation                = ld_i32_be(buffer + 40);
  header->surface_elevation_at_source             = ld_i32_be(buffer + 44);
  header->source_depth_below_surface              = ld_i32_be(buffer + 48);
  header->datum_elevation_at_receiver_group       = ld_i32_be(buffer + 52);
  header->datum_elevation_at_source               = ld_i32_be(buffer + 56);
  header->water_depth_at_source                   = ld_i32_be(buffer + 60);
  header->water_depth_at_group                    = ld_i32_be(buffer + 64);
  header->elevation_scaling                       = ld_i16_be(buffer + 68);
  header->coordinate_scaling                      = ld_i16_be(buffer + 70);
  header->source_coordinate_x                     = ld_i32_be(buffer + 72);
  header->source_coordinate_y                     = ld_i32_be(buffer + 76);
  header->group_coordinate_x                      = ld_i32_be(buffer + 80);
  header->group_coordinate_y                      = ld_i32_be(buffer + 84);
  header->coordinate_units                        = ld_i16_be(buffer + 88);
  header->weather_velocity                        = ld_i16_be(buffer + 90);
  header->subweather_velocity                     = ld_i16_be(buffer + 92);
  header->uphole_time_at_source                   = ld_i16_be(buffer + 94);
  header->uphole_time_at_group                    = ld_i16_be(buffer + 96);
  header->source_static_correction                = ld_i16_be(buffer + 98);
  header->group_static_correction                 = ld_i16_be(buffer + 100);
  header->total_static                            = ld_i16_be(buffer + 102);
  header->lag_time_a                              = ld_i16_be(buffer + 104);
  header->lag_time_b                              = ld_i16_be(buffer + 106);
  header->delay_recording_time                    = ld_i16_be(buffer + 108);
  header->mute_time_start                         = ld_i16_be(buffer + 110);
  header->mute_time_end                           = ld_i16_be(buffer + 112);
  header->num_samples                             = ld_i16_be(buffer + 114);
  header->sample_interval                         = ld_i16_be(buffer + 116);
  header->instrument_gain_type                    = ld_i16_be(buffer + 118);
  header->instrument_gain_constant                = ld_i16_be(buffer + 120);
  header->instrument_gain_initial                 = ld_i16_be(buffer + 122);
  header->correlated                              = ld_i16_be(buffer + 124) == SEGY_YES ? 2 : 1;
  header->sweep_frequency_start                   = ld_i16_be(buffer + 126);
  header->sweep_frequency_end                     = ld_i16_be(buffer + 128);
  header->sweep_length                            = ld_i16_be(buffer + 130);
  header->sweep_type                              = ld_i16_be(buffer + 132);
  header->sweep_trace_taper_length_start          = ld_i16_be(buffer + 134);
  header->sweep_trace_taper_length_end            = ld_i16_be(buffer + 136);
  header->taper_type                              = ld_i16_be(buffer + 138);
  header->alias_filter_frequency                  = ld_i16_be(buffer + 140);
  header->alias_filter_slope                      = ld_i16_be(buffer + 142);
  header->notch_filter_frequency                  = ld_i16_be(buffer + 144);
  header->notch_filter_slope                      = ld_i16_be(buffer + 146);
  header->lowcut_filter_frequency                 = ld_i16_be(buffer + 148);
  header->lowcut_filter_slope                     = ld_i16_be(buffer + 150);
  header->highcut_filter_frequency                = ld_i16_be(buffer + 152);
  header->highcut_filter_slope                    = ld_i16_be(buffer + 154);
  header->year                                    = ld_i16_be(buffer + 156);
  header->julian_day                              = ld_i16_be(buffer + 158);
  header->hour                                    = ld_i16_be(buffer + 160);
  header->minute                                  = ld_i16_be(buffer + 162);
  header->second                                  = ld_i16_be(buffer + 164);
  header->time_basis                              = ld_i16_be(buffer + 166);
  header->trace_weighting_factor                  = ld_i16_be(buffer + 168);
  header->geophone_group_number_of_roll_switch_position_one = ld_i16_be(buffer + 170);
  header->geophone_group_number_of_first_trace_original = ld_i16_be(buffer + 172);
  header->geophone_group_number_of_last_trace_original = ld_i16_be(buffer + 174);
  header->gap_size                                = ld_i16_be(buffer + 176);
  header->taper_over_travel                       = ld_i16_be(buffer + 178);
  header->ensemble_x_coordinate                   = ld_i32_be(buffer + 180);
  header->ensemble_y_coordinate                   = ld_i32_be(buffer + 184);
  header->poststack_3d_inline_number              = ld_i32_be(buffer + 188);
  header->poststack_3d_crossline_number           = ld_i32_be(buffer + 192);
  header->shotpoint_number                        = ld_i32_be(buffer + 196);
  header->shotpoint_scaling                       = ld_i16_be(buffer + 200);
  u = ld_i16_be(buffer + 202);
  switch (u) {
    case -1: header->trace_value_measurement_unit = SEGY_OTHER; break;
    case  0: header->trace_value_measurement_unit = SEGY_UNKNOWN; break;
    case  1: header->trace_value_measurement_unit = SEGY_PASCAL; break;
    case  2: header->trace_value_measurement_unit = SEGY_VOLT; break;
    case  3: header->trace_value_measurement_unit = SEGY_MILLIVOLT; break;
    case  4: header->trace_value_measurement_unit = SEGY_AMPERE; break;
    case  5: header->trace_value_measurement_unit = SEGY_METER; break;
    case  6: header->trace_value_measurement_unit = SEGY_METER_PER_SECOND; break;
    case  7: header->trace_value_measurement_unit = SEGY_METER_PER_SECOND_SQUARED; break;
    case  8: header->trace_value_measurement_unit = SEGY_NEWTON; break;
    case  9: header->trace_value_measurement_unit = SEGY_WATT; break;
    default: header->trace_value_measurement_unit = SEGY_OTHER; break;
  }
  header->transduction_constant_mantissa          = ld_i32_be(buffer + 204);
  header->transduction_constant_exponent          = ld_i16_be(buffer + 208);
  u = ld_i16_be(buffer + 210);
  switch (u) {
    case -1: header->transduction_unit = SEGY_OTHER; break;
    case  0: header->transduction_unit = SEGY_UNKNOWN; break;
    case  1: header->transduction_unit = SEGY_PASCAL; break;
    case  2: header->transduction_unit = SEGY_VOLT; break;
    case  3: header->transduction_unit = SEGY_MILLIVOLT; break;
    case  4: header->transduction_unit = SEGY_AMPERE; break;
    case  5: header->transduction_unit = SEGY_METER; break;
    case  6: header->transduction_unit = SEGY_METER_PER_SECOND; break;
    case  7: header->transduction_unit = SEGY_METER_PER_SECOND_SQUARED; break;
    case  8: header->transduction_unit = SEGY_NEWTON; break;
    case  9: header->transduction_unit = SEGY_WATT; break;
    default: header->transduction_unit = SEGY_OTHER; break;
  }
  header->device_id                               = ld_i16_be(buffer + 212);
  header->time_scaling                            = ld_i16_be(buffer + 214);
  header->source_type                             = ld_i16_be(buffer + 216);
  /* XXX: source_energy_direction. */
  header->source_measurement_mantissa             = ld_i32_be(buffer + 224);
  header->source_measurement_exponent             = ld_i16_be(buffer + 228);
  u = ld_i16_be(buffer + 230);
  switch (u) {
    case -1: header->source_measurement_unit = SEGY_OTHER; break;
    case  0: header->source_measurement_unit = SEGY_UNKNOWN; break;
    case  1: header->source_measurement_unit = SEGY_JOULE; break;
    case  2: header->source_measurement_unit = SEGY_KILOWATT; break;
    case  3: header->source_measurement_unit = SEGY_PASCAL; break;
    case  4: header->source_measurement_unit = SEGY_BAR; break;
    case  5: header->source_measurement_unit = SEGY_BAR_METER; break;
    case  6: header->source_measurement_unit = SEGY_NEWTON; break;
    default: header->source_measurement_unit = SEGY_OTHER; break;
  }
  return 240;
}
