#ifndef WAV_FILE_H
#define WAV_FILE_H

/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>

#include "wav.h"

/* The structure for storing handles to WAV-Files. */
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
  /* Frame buffer. */
  uint8_t *buffer;
};

extern wav_file_t *wav_file_open(const char *path);
extern wav_file_t *wav_file_create(const char *path, unsigned long sample_rate, unsigned int num_channels, unsigned int bit_depth);
extern void wav_file_close(wav_file_t *file);

extern int wav_file_read_int_frame(wav_file_t *file, int32_t *frame);
extern int wav_file_write_int_frame(wav_file_t *file, const int32_t *frame);

extern int wav_file_read_double_frame(wav_file_t *file, double *frame);
extern int wav_file_write_double_frame(wav_file_t *file, const double *frame);

#endif
