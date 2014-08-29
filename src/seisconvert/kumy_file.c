#include "kumy_file.h"

#include <stdlib.h>

kumy_file_t *kumy_file_open(const char *path)
{
  uint8_t ht[KUMY_TEXT_HEADER_BYTES], hb[KUMY_BINARY_HEADER_BYTES];
  int i;
  /* Allocate memory for the handle. */
  kumy_file_t *file = malloc(sizeof(kumy_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "r"))) goto err1;
  file->mode = 0;
  /* Read header. */
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    if (fread(ht, KUMY_TEXT_HEADER_BYTES, 1, file->file_handle) != 1) goto err2;
    if (kumy_text_header_read(&file->text_header[i], ht) < 0) goto err2;
    if (fread(hb, KUMY_BINARY_HEADER_BYTES, 1, file->file_handle) != 1) goto err2;
    if (kumy_binary_header_read(&file->binary_header[i], hb) < 0) goto err2;
  }
  /* Frame config */
  file->frame_config = kumy_get_frame_config(&file->binary_header[0]);
  /* Allocate frame buffer. */
  file->buffer = malloc(kumy_get_frame_size(file->frame_config));
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

kumy_file_t *kumy_file_create(const char *path)
{
  /* Not implemented. */
  return 0;
}

void kumy_file_close(kumy_file_t *file)
{
  if (!file) return;
  free(file->buffer);
  fclose(file->file_handle);
  free(file);
}

int kumy_file_read_int_frame(kumy_file_t *file, int32_t *frame)
{
  int i;
  if (!file || !frame || file->mode != 0) return -3;
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    if (file->binary_header[i].num_samples == 0) return -1;
    if (fread(file->buffer, kumy_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
    kumy_read_int_frame(file->frame_config, frame + i, file->buffer);
    file->binary_header[i].num_samples -= 1;
  }
  return 0;
}

int kumy_file_write_int_frame(kumy_file_t *file, const int32_t *frame)
{
  int i;
  if (!file || !frame || file->mode != 1) return -3;
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    kumy_write_int_frame(file->frame_config, file->buffer, frame + i);
    file->binary_header[i].num_samples += 1;
    if (fwrite(file->buffer, kumy_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  }
  return 0;
}

int kumy_file_read_double_frame(kumy_file_t *file, double *frame)
{
  int i;
  if (!file || !frame || file->mode != 0) return -3;
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    if (file->binary_header[i].num_samples == 0) return -1;
    if (fread(file->buffer, kumy_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -1;
    kumy_read_double_frame(file->frame_config, frame + i, file->buffer);
    file->binary_header[i].num_samples -= 1;
  }
  return 0;
}

int kumy_file_write_double_frame(kumy_file_t *file, const double *frame)
{
  int i;
  if (!file || !frame || file->mode != 1) return -3;
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    kumy_write_double_frame(file->frame_config, file->buffer, frame + i);
    file->binary_header[i].num_samples += 1;
    if (fwrite(file->buffer, kumy_get_frame_size(file->frame_config), 1, file->file_handle) != 1) return -3;
  }
  return 0;
}
