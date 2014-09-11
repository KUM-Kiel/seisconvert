#ifndef WAV_FILE_H
#define WAV_FILE_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>

#include "wav.h"

/* The structure for storing handles to WAV files. */
typedef struct wav_file_s wav_file_t;
struct wav_file_s {
  /* Userdata. */
  void *data;
  /* stdio file handle corresponding to the physicial file. */
  FILE *file_handle;
  /* File mode. 0 for reading, 1 for writing. */
  int mode;
  /* WAV header. */
  wav_header_t header;
  /* WAV frame config. */
  wav_frame_config_t frame_config;
  /* Current position. */
  uint32_t position;
  /* Frame buffer. */
  uint8_t *buffer;
};

/* Opens a WAV file for reading. */
extern wav_file_t *wav_file_open(const char *path);

/* Creates a WAV file for writing with the given sample rate, number of channels
 * and bit depth. */
extern wav_file_t *wav_file_create(const char *path, uint32_t sample_rate, uint16_t num_channels, uint16_t bit_depth);

/* Closes an open WAV file, frees it's resources and writes all pending frames
 * to disk. */
extern void wav_file_close(wav_file_t *file);

/* Returns the sample rate of the WAV file. */
extern uint32_t wav_file_sample_rate(wav_file_t *file);

/* Returns the number of channels of the WAV file. */
extern uint16_t wav_file_num_channels(wav_file_t *file);

/* Returns the bit depth of the WAV file. */
extern uint16_t wav_file_bit_depth(wav_file_t *file);

/* Returns the number of frames in the file. */
extern uint16_t wav_file_num_frames(wav_file_t *file);

/* Returns the position in the file, i.e. which frame will be read next. */
extern uint32_t wav_file_position(wav_file_t *file);

/* Seeks to the specified frame in the file. This works only on files in read mode. */
extern int wav_file_seek(wav_file_t *file, uint32_t frame);

/* Reads a frame from the WAV file and stores it as 32 bit integers.
 * The supplied frame pointer must have enough space for one int32_t per channel.
 * Returns -1 if there is no more data in the file.
 * Returns -3 if there was an error or the file is malformed. */
extern int wav_file_read_int_frame(wav_file_t *file, int32_t *frame);

/* Writes a frame to the WAV file. The values are taken from the supplied frame
 * pointer, one int32_t per channel. */
extern int wav_file_write_int_frame(wav_file_t *file, const int32_t *frame);

/* Exactly like the int functions, but with doubles.
 * The values range from -1.0 to 1.0. */
extern int wav_file_read_double_frame(wav_file_t *file, double *frame);
extern int wav_file_write_double_frame(wav_file_t *file, const double *frame);

/* Exactly like the int functions, but with floats.
 * The values range from -1.0 to 1.0. */
extern int wav_file_read_float_frame(wav_file_t *file, float *frame);
extern int wav_file_write_float_frame(wav_file_t *file, const float *frame);

#endif
