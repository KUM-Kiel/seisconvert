#include "miniseed_file.h"
#include <stdlib.h>
#include "number.h"

static int read_next_record(miniseed_file_t *f)
{
  uint8_t drh[SEED_DATA_RECORD_HEADER_BYTES], b1k[SEED_BLOCKETTE_1000_BYTES];
  f->record_start += f->record_size;
  /* Seek to record start and read. */
  if (fseek(f->file_handle, f->record_start, SEEK_SET) < 0) return -1;
  if (fread(drh, SEED_DATA_RECORD_HEADER_BYTES, 1, f->file_handle) != 1) return -1;
  if (seed_read_data_record_header(&f->record_header, drh) < 0) return -1;
  /* Seek to first blockette and read. */
  if (fseek(f->file_handle, f->record_start + f->record_header.blockette_offset, SEEK_SET) < 0) return -1;
  if (fread(b1k, SEED_BLOCKETTE_1000_BYTES, 1, f->file_handle) != 1) return -1;
  if (seed_read_blockette_1000(&f->blockette_1000, b1k) < 0) return -1;
  /* Size in bytes is 2^(f->data_record_length). */
  f->record_size = 1ULL << f->blockette_1000.data_record_length;
  f->frames_in_record = f->record_header.num_samples;
  f->frame = 0;
  f->next_row = f->record_header.data_offset;
  f->row_word = 16;
  f->word_sample = 0;
  return 0;
}

miniseed_file_t *miniseed_file_open(const char *path)
{
  /* Allocate memory for the handle. */
  miniseed_file_t *file = malloc(sizeof(miniseed_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "r"))) goto err1;
  file->mode = 0;
  /* Read first record. */
  file->record_start = 0;
  file->record_size = 0;
  if (read_next_record(file) < 0) goto err2;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

miniseed_file_t *miniseed_file_create(const char *path);

void miniseed_file_close(miniseed_file_t *file)
{
  if (!file) return;
  fclose(file->file_handle);
  free(file);
}

static int read_next_row(miniseed_file_t *file)
{
  int i;
  uint8_t row[64];
  if (fseek(file->file_handle, file->record_start + file->next_row, SEEK_SET) < 0) return -1;
  if (fread(row, 1, 64, file->file_handle) <= 0) return -1;
  for (i = 0; i < 16; ++i) {
    file->row[i] = ld_i32_be(row + i * 4);
  }
  file->row_word = 0;
  file->next_row += 64;
  file->word_sample = 0;
  return 0;
}

static int samples_in_word(int32_t *row, int word)
{
  int x = (*row >> (30 - 2 * word)) & 3;
  switch (x) {
    case 1: return 4;
    case 2: return 2;
    case 3: return 1;
    default: return 0;
  }
}

static int32_t get_sample(int32_t word, int samples_in_word, int sample)
{
  switch (samples_in_word) {
    case 4: return (int32_t)(word << (8 * sample)) >> 24; /* Two shifts for sign extension. */
    case 2: return (int32_t)(word << (16 * sample)) >> 16; /* Two shifts for sign extension. */
    default: return word;
  }
}

int miniseed_file_read_int_frame(miniseed_file_t *file, int32_t *frame)
{
  int siw;
  /* Read next record, if we have read all frames. */
  if (file->frame == file->frames_in_record) {
    if (read_next_record(file) < 0) return -1;
  }
  /* Read next row, if we have read all words in a row. */
  if (file->row_word == 16) {
    if (read_next_row(file) < 0) return -1;
  }
  /* Read next sample. */
  switch (file->blockette_1000.encoding) {
    case 3: /* 32bit integers. */
      *frame = file->row[file->row_word];
      file->row_word += 1;
      return 0;
    case 10: /* Steim1 compression. */
      if (file->frame == 0) {
        *frame = file->last_sample = file->row[1];
        file->frame += 1;
        file->row_word = 3;
        file->word_sample = 0;
      } else {
        while (samples_in_word(file->row, file->row_word) == 0) {
          file->row_word += 1;
          /* If this happens, the file is malformed. */
          if (file->row_word == 16) return -3;
        }
        siw = samples_in_word(file->row, file->row_word);
        *frame = file->last_sample =
          file->last_sample + get_sample(file->row[file->row_word], siw, file->word_sample);
        file->frame += 1;
        file->word_sample += 1;
        if (file->word_sample == siw) {
          file->word_sample = 0;
          file->row_word += 1;
        }
      }
      return 0;
    default: /* Unsupported. */
      return -3;
  }
}

int miniseed_file_write_int_frame(miniseed_file_t *file, const int32_t *frame);

int miniseed_file_read_double_frame(miniseed_file_t *file, double *frame);

int miniseed_file_write_double_frame(miniseed_file_t *file, const double *frame);