#include "wav_file.h"

#include <stdlib.h>

wav_file_t *wav_file_open(const char *path)
{
  uint8_t h[WAV_HEADER_BYTES];
  /* Allocate memory for the handle. */
  wav_file_t *file = malloc(sizeof(wav_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "r"))) goto err1;
  file->mode = 0;
  /* Read header. */
  if (fread(h, WAV_HEADER_BYTES, 1, file->file_handle) != 1) goto err2;
  if (wav_header_read(&file->header, h) < 0) goto err2;
  file->frame_config = wav_get_frame_config(&file->header);
  /* Allocate frame buffer. */
  file->buffer = malloc(wav_get_frame_size(file->frame_config));
  if (!file->buffer) goto err2;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

wav_file_t *wav_file_create(const char *path, unsigned long sample_rate, unsigned int num_channels, unsigned int bit_depth)
{
  uint8_t h[WAV_HEADER_BYTES];
  /* Allocate memory for the handle. */
  wav_file_t *file = malloc(sizeof(wav_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "w"))) goto err1;
  file->mode = 1;
  /* Setup. */
  file->header.format = WAV_PCM;
  file->header.num_channels = num_channels;
  file->header.sample_rate = sample_rate;
  file->header.bit_depth = bit_depth;
  file->header.num_frames = 0;
  file->frame_config = wav_get_frame_config(&file->header);
  /* Allocate frame buffer. */
  file->buffer = malloc(wav_get_frame_size(file->frame_config));
  if (!file->buffer) goto err2;
  /* Write header. */
  wav_header_write(h, &file->header);
  if (fwrite(h, WAV_HEADER_BYTES, 1, file->file_handle) != 1) goto err3;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err3:
  free(file->buffer);
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

void wav_file_close(wav_file_t *file)
{
  uint8_t h[WAV_HEADER_BYTES];
  if (!file) return;
  if (file->mode == 1) {
    /* Rewrite header with cerrect num_frames. */
    wav_header_write(h, &file->header);
    rewind(file->file_handle);
    fwrite(h, WAV_HEADER_BYTES, 1, file->file_handle);
  }
  free(file->buffer);
  fclose(file->file_handle);
  free(file);
}

int wav_file_read_int_frame(wav_file_t *file, int32_t *frame)
{
  if (!file || !frame) return -3;
  if (fread(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
  wav_read_int_frame(file->frame_config, frame, file->buffer);
  return 0;
}

int wav_file_write_int_frame(wav_file_t *file, const int32_t *frame)
{
  if (!file || !frame) return -3;
  wav_write_int_frame(file->frame_config, file->buffer, frame);
  file->header.num_frames += 1;
  if (fwrite(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  return 0;
}

int wav_file_read_double_frame(wav_file_t *file, double *frame)
{
  if (!file || !frame) return -3;
  if (fread(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
  wav_read_double_frame(file->frame_config, frame, file->buffer);
  return 0;
}

int wav_file_write_double_frame(wav_file_t *file, const double *frame)
{
  if (!file || !frame) return -3;
  wav_write_double_frame(file->frame_config, file->buffer, frame);
  file->header.num_frames += 1;
  if (fwrite(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  return 0;
}

