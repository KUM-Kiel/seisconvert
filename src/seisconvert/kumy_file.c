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

static const uint8_t text_header_base[3200] =
  "C 1                                                                             "
  "C 2                                                                             "
  "C 3                                                                             "
  "C 4                                                                             "
  "C 5                                                                             "
  "C 6                                                                             "
  "C 7                                                                             "
  "C 8                                                                             "
  "C 9                                                                             "
  "C10                                                                             "
  "C11                                                                             "
  "C12                                                                             "
  "C13                                                                             "
  "C14                                                                             "
  "C15                                                                             "
  "C16                                                                             "
  "C17                                                                             "
  "C18                                                                             "
  "C19                                                                             "
  "C20                                                                             "
  "C21                                                                             "
  "C22                                                                             "
  "C23 START OF KUM HEADER                                                         "
  "C24 START RECORDING TIME                                                        "
  "C25 STOP RECORDING TIME                                                         "
  "C26 SYNCHRONISATION TIME                                                        "
  "C27 SKEW TIME                                                                   "
  "C28 SKEW IN MICROSECS                                                           "
  "C29 SAMPLE PERIOD IN MICROSECS                                                  "
  "C30 CHANNEL NO                                                                  "
  "C31 CHANNEL GAIN                                                                "
  "C32                                                                             "
  "C33                                                                             "
  "C34                                                                             "
  "C35                                                                             "
  "C36                                                                             "
  "C37                                                                             "
  "C38                                                                             "
  "C39                                                                             "
  "C40 END TEXTUAL HEADER                                                          ";

kumy_file_t *kumy_file_create(const char *path, uint32_t sample_rate)
{
  uint8_t ht[KUMY_TEXT_HEADER_BYTES], hb[KUMY_BINARY_HEADER_BYTES];
  int i, j;
  /* Allocate memory for the handle. */
  kumy_file_t *file = malloc(sizeof(kumy_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "w"))) goto err1;
  file->mode = 1;
  /* Fill header. */
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    file->binary_header[i].job_id = 1;
    file->binary_header[i].year = 0;
    file->binary_header[i].julian_day = 0;
    file->binary_header[i].recorder_no = 1;
    file->binary_header[i].channel_no = i;
    file->binary_header[i].sample_interval = 1000000 / sample_rate;
    file->binary_header[i].num_samples = 0;
    for (j = 0; j < 3200; ++j) {
      file->text_header[i].content[j] = text_header_base[j];
    }
  }
  /* Write header. */
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    if (kumy_text_header_write(ht, &file->text_header[i]) < 0) goto err2;
    if (fwrite(ht, KUMY_TEXT_HEADER_BYTES, 1, file->file_handle) != 1) goto err2;
    if (kumy_binary_header_write(hb, &file->binary_header[i]) < 0) goto err2;
    if (fwrite(hb, KUMY_BINARY_HEADER_BYTES, 1, file->file_handle) != 1) goto err2;
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

void kumy_file_close(kumy_file_t *file)
{
  uint8_t ht[KUMY_TEXT_HEADER_BYTES], hb[KUMY_BINARY_HEADER_BYTES];
  int i = 0;
  if (!file) return;
  if (file->mode == 1) {
    fseek(file->file_handle, 0, SEEK_SET);
    for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
      if (kumy_text_header_write(ht, &file->text_header[i]) < 0) break;
      if (fwrite(ht, KUMY_TEXT_HEADER_BYTES, 1, file->file_handle) != 1) break;
      if (kumy_binary_header_write(hb, &file->binary_header[i]) < 0) break;
      if (fwrite(hb, KUMY_BINARY_HEADER_BYTES, 1, file->file_handle) != 1) break;
    }
  }
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
