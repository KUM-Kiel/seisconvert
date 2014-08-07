#ifndef SEGY_H
#define SEGY_H

#include <stdint.h>

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

#define SEGY_NO 0
#define SEGY_YES 1

#define SEGY_METER 1
#define SEGY_FEET 2

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

#endif
