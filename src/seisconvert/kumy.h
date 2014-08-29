#ifndef KUMY_H
#define KUMY_H

#include <stdint.h>

/* Structure for binary headers. */
#define KUMY_BINARY_HEADER_BYTES 400
struct kumy_binary_header_s {
  uint32_t job_id;
  uint32_t year;
  uint32_t julian_day;
  uint32_t recorder_no;
  uint32_t channel_no;
  uint32_t sample_interval;
  uint32_t num_samples;
};
typedef struct kumy_binary_header_s kumy_binary_header_t;

/* Reads a kumy_binary_header_t from a 400 byte uint8_t buffer.
 * Returns -1 if the buffer is malformed. */
extern int kumy_binary_header_read(kumy_binary_header_t *header, const uint8_t *buffer);

/* Writes a kumy_binary_header_t to a 400 byte uint8_t buffer. */
extern int kumy_binary_header_write(uint8_t *buffer, kumy_binary_header_t *header);

/* Structure for text headers. */
#define KUMY_TEXT_HEADER_BYTES 3200
struct kumy_text_header_s {
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
typedef struct kumy_text_header_s kumy_text_header_t;

/* Reads a kumy_text_header_t from a 3200 byte uint8_t buffer.
 * Returns -1 if the buffer is malformed. */
extern int kumy_text_header_read(kumy_text_header_t *header, const uint8_t *buffer);

/* Writes a kumy_text_header_t to a 3200 byte uint8_t buffer. */
extern int kumy_text_header_write(uint8_t *buffer, kumy_text_header_t *header);

typedef uint32_t kumy_frame_config_t;

extern kumy_frame_config_t kumy_get_frame_config(kumy_binary_header_t *header);

extern int kumy_get_frame_size(kumy_frame_config_t fc);

extern int kumy_read_int_frame(kumy_frame_config_t fc, int32_t *samples, const uint8_t *buffer);
extern int kumy_write_int_frame(kumy_frame_config_t fc, uint8_t *buffer, const int32_t *samples);

extern int kumy_read_double_frame(kumy_frame_config_t fc, double *samples, const uint8_t *buffer);
extern int kumy_write_double_frame(kumy_frame_config_t fc, uint8_t *buffer, const double *samples);

#endif
