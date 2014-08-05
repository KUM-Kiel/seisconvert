#ifndef KUM_SEGY_H
#define KUM_SEGY_H

#include <stdint.h>

struct kum_segy_binary_header_s {
  uint32_t job_id;
  uint32_t year;
  uint32_t julian_day;
  uint32_t recorder_no;
  uint32_t channel_no;
  uint32_t sample_interval;
  uint32_t num_samples;
};
typedef struct kum_segy_binary_header_s kum_segy_binary_header_t;

/* Reads a kum_segy_binary_header_t from a 400 byte uint8_t buffer.
   Returns -1 if the buffer is malformed. */
extern int kum_segy_binary_header_read(kum_segy_binary_header_t *header, uint8_t *buffer);

/* Writes a kum_segy_binary_header_t to a 400 byte uint8_t buffer. */
extern int kum_segy_binary_header_write(uint8_t *buffer, kum_segy_binary_header_t *header);

struct kum_segy_text_header_s {
  uint8_t content[3200];
  /* C1 */
  uint8_t client[23];        /* 11 */
  uint8_t company[23];       /* 42 */
  uint8_t crew_no[9];        /* 72 */
  /* C2 */
  uint8_t line[12];          /* 89 */
  uint8_t area[25];          /* 105 */
  /* C3 */
  uint8_t reel_day_start[4]; /* 200 */
  uint8_t reel_year[5];      /* 209 */
  uint8_t observer[18];      /* 223 */
  /* C4 */
  uint8_t instrument[16];    /* 256 */
  uint8_t model[12];         /* 277 */
  uint8_t serial_no[23];     /* 298 */
};
typedef struct kum_segy_text_header_s kum_segy_text_header_t;

/* Reads a kum_segy_text_header_t from a 3200 byte uint8_t buffer.
   Returns -1 if the buffer is malformed. */
extern int kum_segy_text_header_read(kum_segy_text_header_t *header, uint8_t *buffer);

/* Writes a kum_segy_text_header_t to a 3200 byte uint8_t buffer. */
extern int kum_segy_text_header_write(uint8_t *buffer, kum_segy_text_header_t *header);

typedef uint32_t kum_segy_frame_config_t;

extern kum_segy_frame_config_t kum_segy_get_frame_config(kum_segy_binary_header_t *header);

extern int kum_segy_get_frame_size(kum_segy_frame_config_t fc);

extern int kum_segy_read_int_frame(kum_segy_frame_config_t fc, int32_t *samples, uint8_t *buffer);
extern int kum_segy_write_int_frame(kum_segy_frame_config_t fc, uint8_t *buffer, int32_t *samples);

#endif
