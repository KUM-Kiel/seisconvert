#ifndef SEGY_H
#define SEGY_H

#include <stdint.h>

/* General stuff. */
#define SEGY_NO 0
#define SEGY_YES 1

/* Beware, these numbers differ from the numbers written to the header.
 * Always use these named constants, as they are remapped to the appropiate numbers. */
#define SEGY_OTHER -1
#define SEGY_UNKNOWN 0

#define SEGY_VOLT 2
#define SEGY_MILLIVOLT 3
#define SEGY_AMPERE 4

#define SEGY_METER 5
#define SEGY_METER_PER_SECOND 6
#define SEGY_METER_PER_SECOND_SQUARED 7
#define SEGY_FEET 10

#define SEGY_NEWTON 8

#define SEGY_JOULE 11
#define SEGY_WATT 9
#define SEGY_KILOWATT 12

#define SEGY_PASCAL 1
#define SEGY_BAR 13

#define SEGY_KILOGRAM 14

typedef struct segy_text_header_s segy_text_header_t;
struct segy_text_header_s {
  uint8_t content[3200];
};

/* Reads a segy_text_header_t from a 3200 byte uint8_t buffer. */
extern int segy_text_header_read(segy_text_header_t *header, const uint8_t *buffer);

/* Writes a segy_text_header_t to a 3200 byte uint8_t buffer. */
extern int segy_text_header_write(uint8_t *buffer, segy_text_header_t *header);

typedef struct segy_binary_header_s segy_binary_header_t;
struct segy_binary_header_s {
  int32_t job_id;
  int32_t line_number;
  int32_t reel_number;
  int16_t data_traces_per_ensemble;
  int16_t auxiliary_traces_per_ensemble;
  int16_t sample_interval;                 /* µs */
  int16_t sample_interval_original;        /* µs */
  int16_t samples_per_data_trace;
  int16_t samples_per_data_trace_original;
  int16_t format;                          /* SEGY_FORMAT_* */
  int16_t ensemble_fold;
  int16_t trace_sorting;                   /* SEGY_SORTING_* */
  int16_t vertical_sum;

  int16_t sweep_frequency_start;           /* Hz */
  int16_t sweep_frequency_end;             /* Hz */
  int16_t sweep_length;                    /* ms */
  int16_t sweep_type;                      /* SEGY_SWEEP_* */
  int16_t sweep_channel_trace_number;
  int16_t sweep_trace_taper_length_start;  /* ms */
  int16_t sweep_trace_taper_length_end;    /* ms */
  int16_t taper_type;                      /* SEGY_TAPER_* */

  int16_t correlated_data_traces;          /* SEGY_YES or SEGY_NO */
  int16_t binary_gain_recovered;           /* SEGY_YES or SEGY_NO */
  int16_t amplitude_recovery;              /* SEGY_AMPLITUDE_RECOVERY_* */
  int16_t measurement_system;              /* SEGY_METER or SEGY_FEET */
  int16_t signal_polarity;                 /* SEGY_POLARITY_* */
  int16_t vibratory_polarity;              /* 0°–359° */
  int16_t fixed_trace_length;              /* SEGY_YES or SEGY_NO */
  int16_t num_extended_textual_headers;
};

#define SEGY_FORMAT_IBM 1
#define SEGY_FORMAT_I32 2
#define SEGY_FORMAT_I16 3
#define SEGY_FORMAT_FIXED_GAIN 4
#define SEGY_FORMAT_FLOAT 5
#define SEGY_FORMAT_I8 8

#define SEGY_SORTING_OTHER -1
#define SEGY_SORTING_UNKNOWN 0
#define SEGY_SORTING_AS_RECORDED 1
#define SEGY_SORTING_CDP 2
#define SEGY_SORTING_SINGLE_FOLD 3
#define SEGY_SORTING_HORIZONTAL 4
#define SEGY_SORTING_COMMON_SOURCE_POINT 5
#define SEGY_SORTING_COMMON_RECEIVER_POINT 6
#define SEGY_SORTING_COMMON_OFFSET_POINT 7
#define SEGY_SORTING_COMMON_MID_POINT 8
#define SEGY_SORTING_COMMON_CONVERSION_POINT 9

#define SEGY_SWEEP_LINEAR 1
#define SEGY_SWEEP_PARABOLIC 2
#define SEGY_SWEEP_EXPONENTIAL 3
#define SEGY_SWEEP_OTHER 4

#define SEGY_TAPER_LINEAR 1
#define SEGY_TAPER_COS 2
#define SEGY_TAPER_OTHER 3

#define SEGY_AMPLITUDE_RECOVERY_NONE 1
#define SEGY_AMPLITUDE_RECOVERY_SPHERICAL_DIVERGENCE 2
#define SEGY_AMPLITUDE_RECOVERY_AGC 3
#define SEGY_AMPLITUDE_RECOVERY_OTHER 4

#define SEGY_POLARITY_DOWN 1
#define SEGY_POLARITY_UP 2

/* Reads a segy_binary_header_t from a 400 byte uint8_t buffer.
   Returns -1 if the buffer is malformed. */
extern int segy_binary_header_read(segy_binary_header_t *header, const uint8_t *buffer);

/* Writes a segy_binary_header_t to a 400 byte uint8_t buffer. */
extern int segy_binary_header_write(uint8_t *buffer, segy_binary_header_t *header);

typedef struct segy_trace_header_s segy_trace_header_t;
struct segy_trace_header_S {
  int32_t sequence_number_in_line;
  int32_t sequence_number_in_file;
  int32_t original_field_recor_number;
  int32_t original_trace_number;
  int32_t energy_source_point_number;
  int32_t ensemble_number;
  int32_t trace_number_in_ensemble;
  int16_t trace_identification;            /* SEGY_TRACE_IDENTIFICATION_* */
  int16_t number_of_vertically_summed_traces;
  int16_t number_of_horizontally_stacked_traces;
  int16_t data_use; /* SEGY_DATAUSE_PRODUCTION or SEGY_DATAUSE_TEST */

  /* Scalings are defined as integers. Positive scalings indicate multiplication
   * whereas negative scalings indicate division. */
  int16_t elevation_scaling;               /* Moved here for clarity. It actually stands at byte 68. */
  /* Values scaled by elevation_scaling. */
  int32_t distance_from_source_to_receiver_group;
  int32_t receiver_group_elevation;
  int32_t surface_elevation_at_source;
  int32_t source_depth_below_surface;
  int32_t datum_elevation_at_receiver_group;
  int32_t datum_elevation_at_source;
  int32_t water_depth_at_source;
  int32_t water_depth_at_group;

  int16_t coordinate_scaling;
  /* Values scaled by coordinate_scaling. */
  int32_t source_coordinate_x;
  int32_t source_coordinate_y;
  int32_t group_coordinate_x;
  int32_t group_coordinate_y;
  int16_t coordinate_units;                /* SEGY_COORDINATE_UNIT_* */

  /* m/s or ft/s as specified in binar header (measurement_system). */
  int16_t weather_velocity;
  int16_t subweather_velocity;

  int16_t time_scaling;                    /* Actually much later in header! (Byte 214) */
  /* ms scaled by time_scaling */
  int16_t uphole_time_at_source;
  int16_t uphole_time_at_group;
  int16_t source_static_correction;
  int16_t group_static_correction;
  int16_t total_static;
  int16_t lag_time_a;
  int16_t lag_time_b;
  int16_t delay_recording_time;
  int16_t mute_time_start;
  int16_t mute_time_end;

  int16_t num_samples;
  int16_t sample_interval;

  int16_t instrument_gain_type;            /* SEGY_GAIN_TYPE_* */
  int16_t instrument_gain_constant;        /* dB */
  int16_t instrument_gain_initial;         /* dB */
  int16_t correlated;                      /* SEGY_YES or SEGY_NO */

  int16_t sweep_frequency_start;           /* Hz */
  int16_t sweep_frequency_end;             /* Hz */
  int16_t sweep_length;                    /* ms */
  int16_t sweep_type;                      /* SEGY_SWEEP_* */
  int16_t sweep_trace_taper_length_start;  /* ms */
  int16_t sweep_trace_taper_length_end;    /* ms */
  int16_t taper_type;                      /* SEGY_TAPER_* */

  int16_t alias_filter_frequency;          /* Hz */
  int16_t alias_filter_slope;              /* dB/Octave */

  int16_t notch_filter_frequency;          /* Hz */
  int16_t notch_filter_slope;              /* dB/Octave */

  int16_t lowcut_filter_frequency;         /* Hz */
  int16_t lowcut_filter_slope;             /* dB/Octave */

  int16_t highcut_filter_frequency;        /* Hz */
  int16_t highcut_filter_slope;            /* dB/Octave */

  int16_t year;
  int16_t julian_day;
  int16_t hour;
  int16_t minute;
  int16_t second;
  int16_t time_basis;

  int16_t trace_weighting_factor;

  int16_t geophone_group_number_of_roll_switch_position_one;
  int16_t geophone_group_number_of_first_trace_original;
  int16_t geophone_group_number_of_last_trace_original;

  int16_t gap_size;                        /* Total number of groups dropped. */

  int16_t taper_over_travel;               /* SEGY_OVER_TRAVEL_* */

  int32_t ensemble_x_coordinate;
  int32_t ensemble_y_coordinate;
  int32_t poststack_3d_inline_number;
  int32_t poststack_3d_crossline_number;
  int32_t shotpoint_number;
  int16_t shotpoint_scaling;

  int16_t trace_value_measurement_unit;
  int32_t transduction_constant_mantissa;
  int16_t transduction_constant_exponent;
  int16_t transduction_unit;

  int16_t device_id;

  /* time_scaling moved up. */

  int16_t source_type;                     /* SEGY_SOURCE_TYPE_* */
  /* XXX: Crosscheck. */
  int32_t source_energy_direction;         /* In tenths of degrees. 6 Bytes??? */
  int32_t source_measurement_mantissa;
  int16_t source_measurement_exponent;
  int16_t source_measurement_unit;
};

#define SEGY_TRACE_IDENTIFICATION_OTHER -1
#define SEGY_TRACE_IDENTIFICATION_UNKNOWN 0
#define SEGY_TRACE_IDENTIFICATION_SEISMIC 1
#define SEGY_TRACE_IDENTIFICATION_DEAD 2
#define SEGY_TRACE_IDENTIFICATION_DUMMY 3
#define SEGY_TRACE_IDENTIFICATION_TIME_BREAK 4
#define SEGY_TRACE_IDENTIFICATION_UPHOLE 5
#define SEGY_TRACE_IDENTIFICATION_SWEEP 6
#define SEGY_TRACE_IDENTIFICATION_TIMING 7
#define SEGY_TRACE_IDENTIFICATION_WATERBREAK 8
#define SEGY_TRACE_IDENTIFICATION_NEAR_FIELD_GUN 9
#define SEGY_TRACE_IDENTIFICATION_FAR_FIELD_GUN 10
#define SEGY_TRACE_IDENTIFICATION_SEISMIC_PRESSURE 11
#define SEGY_TRACE_IDENTIFICATION_SEISMIC_VERTICAL 12
#define SEGY_TRACE_IDENTIFICATION_SEISMIC_CROSSLINE 13
#define SEGY_TRACE_IDENTIFICATION_SEISMIC_INLINE 14
#define SEGY_TRACE_IDENTIFICATION_ROTATED_SEISMIC_VERTICAL 15
#define SEGY_TRACE_IDENTIFICATION_ROTATED_SEISMIC_CROSSLINE 16
#define SEGY_TRACE_IDENTIFICATION_ROTATED_SEISMIC_INLINE 17
#define SEGY_TRACE_IDENTIFICATION_VIBRATOR_REACTION_MASS 18
#define SEGY_TRACE_IDENTIFICATION_VIBRATOR_BASEPLATE 19
#define SEGY_TRACE_IDENTIFICATION_VIBRATOR_GROUND_FORCE 20
#define SEGY_TRACE_IDENTIFICATION_VIBRATOR_REFERENCE 21
#define SEGY_TRACE_IDENTIFICATION_TIME_VELOCITY 22

#define SEGY_DATAUSE_PRODUCTION 1
#define SEGY_DATAUSE_TEST 2

#define SEGY_COORDINATE_UNIT_LENGTH 1
#define SEGY_COORDINATE_UNIT_ARCSECONDS 2
#define SEGY_COORDINATE_UNIT_DEGREES 3
#define SEGY_COORDINATE_UNIT_DEGREES_MINUTES_SECONDS 4

#define SEGY_GAIN_TYPE_FIXED 1
#define SEGY_GAIN_TYPE_BINARY 2
#define SEGY_GAIN_TYPE_FLOATING_POINT 3

#define SEGY_OVER_TRAVEL_UP 1
#define SEGY_OVER_TRAVEL_DOWN 2

#endif
