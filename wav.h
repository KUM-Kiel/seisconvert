#ifndef WAV_H
#define WAV_H

#include <stdint.h>

#define WAV_PCM 1

struct wav_header_s {
  uint16_t format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint16_t bit_depth;
  uint32_t num_frames;
};
typedef struct wav_header_s wav_header_t;

#define WAV_HEADER_BYTES 44

/* Reads a wav_header_t from a 44 byte uint8_t buffer.
   Returns -1 if the buffer is malformed. */
extern int wav_header_read(wav_header_t *header, uint8_t *buffer);

/* Writes a wav_header_t to a 44 byte uint8_t buffer. */
extern int wav_header_write(uint8_t *buffer, wav_header_t *header);

typedef uint32_t wav_frame_config_t;

extern wav_frame_config_t wav_get_frame_config(wav_header_t *header);

/* Returns the frame size for the given frame config. */
extern int wav_get_frame_size(wav_frame_config_t fc);

extern int wav_read_int_frame(wav_frame_config_t fc, int32_t *samples, const uint8_t *buffer);
extern int wav_write_int_frame(wav_frame_config_t fc, uint8_t *buffer, const int32_t *samples);

extern int wav_read_double_frame(wav_frame_config_t fc, double *samples, const uint8_t *buffer);
extern int wav_write_double_frame(wav_frame_config_t fc, uint8_t *buffer, const double *samples);

#endif
