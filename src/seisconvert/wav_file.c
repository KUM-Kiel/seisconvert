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
  file->position = 0;
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

wav_file_t *wav_file_create(const char *path, uint32_t sample_rate, uint16_t num_channels, uint16_t bit_depth)
{
  uint8_t h[WAV_HEADER_BYTES];
  /* Allocate memory for the handle. */
  wav_file_t *file = malloc(sizeof(wav_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "w"))) goto err1;
  file->mode = 1;
  file->position = 0;
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
    /* Rewrite header with correct num_frames. */
    wav_header_write(h, &file->header);
    rewind(file->file_handle);
    fwrite(h, WAV_HEADER_BYTES, 1, file->file_handle);
  }
  free(file->buffer);
  fclose(file->file_handle);
  free(file);
}

uint32_t wav_file_sample_rate(wav_file_t *file)
{
  if (!file) return 0;
  return file->header.sample_rate;
}

uint16_t wav_file_num_channels(wav_file_t *file)
{
  if (!file) return 0;
  return file->header.num_channels;
}

uint16_t wav_file_bit_depth(wav_file_t *file)
{
  if (!file) return 0;
  return file->header.bit_depth;
}

uint16_t wav_file_num_frames(wav_file_t *file)
{
  if (!file) return 0;
  return file->header.num_frames;
}

uint32_t wav_file_position(wav_file_t *file)
{
  if (!file) return 0;
  return file->position;
}

int wav_file_seek(wav_file_t *file, uint32_t frame)
{
  if (!file || file->mode != 0) return -3;
  if (frame > file->header.num_frames)
    frame = file->header.num_frames;
  if (fseek(file->file_handle, WAV_HEADER_BYTES + frame * wav_get_frame_size(file->frame_config), SEEK_SET) < 0) return -3;
  file->position = frame;
  return 0;
}

int wav_file_read_int_frame(wav_file_t *file, int32_t *frame)
{
  if (!file || !frame || file->mode != 0) return -3;
  if (file->header.num_frames == file->position) return -1;
  if (fread(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
  wav_read_int_frame(file->frame_config, frame, file->buffer);
  file->position += 1;
  return 0;
}

int wav_file_write_int_frame(wav_file_t *file, const int32_t *frame)
{
  if (!file || !frame || file->mode != 1) return -3;
  wav_write_int_frame(file->frame_config, file->buffer, frame);
  file->header.num_frames += 1;
  file->position += 1;
  if (fwrite(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  return 0;
}

int wav_file_read_double_frame(wav_file_t *file, double *frame)
{
  if (!file || !frame || file->mode != 0) return -3;
  if (file->header.num_frames == file->position) return -1;
  if (fread(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
  wav_read_double_frame(file->frame_config, frame, file->buffer);
  file->position += 1;
  return 0;
}

int wav_file_write_double_frame(wav_file_t *file, const double *frame)
{
  if (!file || !frame || file->mode != 1) return -3;
  wav_write_double_frame(file->frame_config, file->buffer, frame);
  file->header.num_frames += 1;
  file->position += 1;
  if (fwrite(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  return 0;
}

int wav_file_read_float_frame(wav_file_t *file, float *frame)
{
  if (!file || !frame || file->mode != 0) return -3;
  if (file->header.num_frames == file->position) return -1;
  if (fread(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
  wav_read_float_frame(file->frame_config, frame, file->buffer);
  file->position += 1;
  return 0;
}

int wav_file_write_float_frame(wav_file_t *file, const float *frame)
{
  if (!file || !frame || file->mode != 1) return -3;
  wav_write_float_frame(file->frame_config, file->buffer, frame);
  file->header.num_frames += 1;
  file->position += 1;
  if (fwrite(file->buffer, wav_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  return 0;
}
