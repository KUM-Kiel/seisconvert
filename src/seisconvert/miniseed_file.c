#include "miniseed_file.h"
#include <stdlib.h>
#include "number.h"

/* Copy l bytes from `from` to `to`. Appends 0 byte to `to`. */
static void byte_copy_0(uint8_t *to, uint64_t l, const uint8_t *from)
{
  uint64_t i;
  for (i = 0; i < l; ++i) to[i] = from[i];
  to[i] = 0;
}

/* Sets l bytes in `to` to `v`. */
static void byte_set(uint8_t *to, uint64_t l, uint8_t v)
{
  uint64_t i;
  for (i = 0; i < l; ++i) to[i] = v;
}

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

static int flush_record(miniseed_file_t *file)
{
  int i;
  if (file->record_header.num_samples == 0) return 0;
  /* Write headers. */
  seed_write_data_record_header(file->write_buffer, &file->record_header);
  seed_write_blockette_1000(file->write_buffer + 0x30, &file->blockette_1000);
  /* Set unused space to 0. */
  byte_set(file->write_buffer + 0x38, 8, 0);
  byte_set(file->write_buffer + file->write_pos, 4096 - file->write_pos, 0);
  /* Write the rcord to disk. */
  if (fwrite(file->write_buffer, 4096, 1, file->file_handle) != 1) return -1;
  /* Change headers for next record. */
  file->write_pos = 0x40;
  file->record_header.sequence_number += 1;
  /* Calculate start time of next record (sample_interval * num_samples). */
  for (i = 0; i < file->record_header.num_samples; ++i) {
    taia_add(&file->record_header.start_time, &file->record_header.start_time, &file->sample_interval);
  }
  file->record_header.num_samples = 0;
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

miniseed_file_t *miniseed_file_create(const char *path)
{
  /* Allocate memory for the handle. */
  miniseed_file_t *file = malloc(sizeof(miniseed_file_t));
  if (!file) return 0;
  /* Try to open the file. */
  if (!(file->file_handle = fopen(path, "w"))) goto err1;
  file->mode = 1;
  /* Allocate write buffer. */
  if (!(file->write_buffer = malloc(4096))) goto err2;
  /* Fill header with stuff. */
  file->record_header.sequence_number = 1;
  byte_copy_0(file->record_header.station_identifier, 5, (uint8_t*)"     ");
  byte_copy_0(file->record_header.location_identifier, 2, (uint8_t*)"  ");
  byte_copy_0(file->record_header.channel_identifier, 3, (uint8_t*)"   ");
  byte_copy_0(file->record_header.network_code, 2, (uint8_t*)"  ");
  file->record_header.num_samples = 0;
  file->record_header.activity_flags = 0;
  file->record_header.io_flags = 0;
  file->record_header.data_quality_flags = 0;
  file->record_header.blockette_count = 1;
  file->record_header.time_correction = 0;
  file->record_header.data_offset = 0x40;
  file->record_header.blockette_offset = 0x30;
  file->blockette_1000.next_blockette = 0;
  file->blockette_1000.encoding = 10; /* Steim1 */
  file->blockette_1000.word_order = 1; /* big endian */
  file->blockette_1000.data_record_length = 12; /* 4096 bytes */
  file->write_pos = 0x40;
  /* Return handle. */
  return file;
  /* Undo everything in case of an error. */
err2:
  fclose(file->file_handle);
err1:
  free(file);
  return 0;
}

void miniseed_file_close(miniseed_file_t *file)
{
  if (!file) return;
  if (file->mode == 1) {
    flush_record(file);
    free(file->write_buffer);
  }
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
      file->frame += 1;
      return 0;
    case 10: /* Steim1 compression. */
      if (file->frame == 0) {
        *frame = file->last_sample = file->row[1];
        file->frame += 1;
        file->row_word = 3;
        file->word_sample = 1;
        if (file->word_sample == samples_in_word(file->row, file->row_word)) {
          file->word_sample = 0;
          file->row_word += 1;
        }
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

static uint64_t row_begin(uint64_t pos)
{
  return pos & ~0x3F;
}

static int bytes_needed(int32_t s)
{
  if (-128 <= s && s < 128)
    return 1;
  else if (-32768 <= s && s < 32768)
    return 2;
  else
    return 4;
}

static void set_samples_in_word(uint8_t *row, int word, int samples)
{
  int x, ls;
  switch (samples) {
    case 4: x = 1; break;
    case 2: x = 2; break;
    case 1: x = 3; break;
    case 0: x = 0; break;
    default: return;
  }
  ls = 30 - 2 * word;
  st_i32_be(row, (ld_i32_be(row) & ~(3 << ls)) | (x << ls));
}

static int combine(uint8_t *b, uint64_t p1, uint64_t p2)
{
  int32_t s1, s2, s3, s4, r1, r2;
  int w1, w2, sw1, sw2;
  w1 = (p1 % 64) / 4;
  w2 = (p2 % 64) / 4;
  r1 = ld_i32_be(b + row_begin(p1));
  r2 = ld_i32_be(b + row_begin(p2));
  sw1 = samples_in_word(&r1, w1);
  sw2 = samples_in_word(&r2, w2);
  if (sw1 > 2 || sw1 != sw2)
    return -1;
  if (sw1 == 1) {
    s1 = ld_i32_be(b + p1);
    s2 = ld_i32_be(b + p2);
    if (bytes_needed(s1) > 2 || bytes_needed(s2) > 2)
      return -1;
    st_i16_be(b + p1, s1);
    st_i16_be(b + p1 + 2, s2);
    st_i32_be(b + p2, 0);
    set_samples_in_word(b + row_begin(p1), w1, 2);
    set_samples_in_word(b + row_begin(p2), w2, 0);
    return 0;
  } else if (sw1 == 2) {
    s1 = ld_i16_be(b + p1);
    s2 = ld_i16_be(b + p1 + 2);
    s3 = ld_i16_be(b + p2);
    s4 = ld_i16_be(b + p2 + 2);
    if (bytes_needed(s1) > 1 || bytes_needed(s2) > 1 ||
        bytes_needed(s3) > 1 || bytes_needed(s4) > 1)
      return -1;
    b[p1] = s1;
    b[p1 + 1] = s2;
    b[p1 + 2] = s3;
    b[p1 + 3] = s4;
    set_samples_in_word(b + row_begin(p1), w1, 4);
    set_samples_in_word(b + row_begin(p2), w2, 0);
    return 0;
  }
  return -1;
}

int miniseed_file_write_int_frame(miniseed_file_t *file, const int32_t *frame)
{
  uint64_t pp;

  if (!file) return -3;
  switch (file->blockette_1000.encoding) {
    case 3: /* 32bit integers. */
      if (file->record_header.num_samples >= 1008)
        flush_record(file);
      st_i32_be(file->write_buffer + file->write_pos, *frame);
      file->write_pos += 4;
      file->record_header.num_samples += 1;
      return 0;
    case 10: /* Steim1 compression. */
      /* Check if a new record is needed. */
      if (file->write_pos == 4096) {
        flush_record(file);
      }
      /* Write the sample. */
      if (file->record_header.num_samples == 0) {
        st_i32_be(file->write_buffer + 0x40, 0);
        st_i32_be(file->write_buffer + 0x44, *frame);
        st_i32_be(file->write_buffer + 0x48, *frame);
        st_i32_be(file->write_buffer + 0x4c, *frame);
        set_samples_in_word(file->write_buffer + 0x40, 3, 1);
        file->write_pos = 0x50;
        file->last_sample = *frame;
        file->record_header.num_samples += 1;
      } else {
        if (file->write_pos == row_begin(file->write_pos)) {
          st_i32_be(file->write_buffer + file->write_pos, 0);
          file->write_pos += 4;
        }
        st_i32_be(file->write_buffer + 0x48, *frame);
        st_i32_be(file->write_buffer + file->write_pos, *frame - file->last_sample);
        pp = row_begin(file->write_pos);
        set_samples_in_word(file->write_buffer + pp, (file->write_pos - pp) / 4, 1);
        file->last_sample = *frame;
        file->record_header.num_samples += 1;
        /* Try combining samples. */
        if (file->record_header.num_samples >= 2) {
          pp = file->write_pos - 4;
          if (pp % 64 == 0)
            pp -= 4;
          if (combine(file->write_buffer, pp, file->write_pos) == 0)
            file->write_pos = pp;
          if (file->record_header.num_samples >= 4) {
            pp = file->write_pos - 4;
            if (pp % 64 == 0)
              pp -= 4;
            if (combine(file->write_buffer, pp, file->write_pos) == 0)
              file->write_pos = pp;
          }
        }
        file->write_pos += 4;
      }
      return 0;
    default: /* Unsupported. */
      return -3;
  }
}

int miniseed_file_read_double_frame(miniseed_file_t *file, double *frame)
{
  int32_t f;
  int r = miniseed_file_read_int_frame(file, &f);
  if (r >= 0)
    *frame = f * (1.0 / 0x7fffffff);
  return r;
}

int miniseed_file_write_double_frame(miniseed_file_t *file, const double *frame)
{
  int32_t f = *frame * 0x7fffffff;
  return miniseed_file_write_int_frame(file, &f);
}

int miniseed_file_set_start_time(miniseed_file_t *file, struct taia *t)
{
  if (!file) return -1;
  file->record_header.start_time = *t;
  return 0;
}

int miniseed_file_set_sample_rate(miniseed_file_t *file, uint32_t sample_rate)
{
  int16_t f, m;
  uint64_t n, a;
  if (!file) return -1;
  seed_sample_rate_from_int(sample_rate, &f, &m);
  a = 1000000000000000000ULL / sample_rate;
  n = a / 1000000000ULL;
  a = a % 1000000000ULL;
  file->record_header.sample_rate_factor = f;
  file->record_header.sample_rate_multiplier = m;
  file->sample_interval.sec.x = 0;
  file->sample_interval.nano = n;
  file->sample_interval.atto = a;
  return 0;
}

int miniseed_file_set_info(miniseed_file_t *file, const char *station, const char *location, const char *channel, const char *network)
{
  int i, _0;
  if (!file) return -1;
  /* Set station_identifier. */
  for (i = 0, _0 = 0; i < 5; ++i) {
    if (!_0 && station && station[i]) {
      file->record_header.station_identifier[i] = station[i];
    } else {
      _0 = 1;
      file->record_header.station_identifier[i] = ' ';
    }
  }
  file->record_header.station_identifier[5] = 0;
  /* Set location_identifier. */
  for (i = 0, _0 = 0; i < 2; ++i) {
    if (!_0 && location && location[i]) {
      file->record_header.location_identifier[i] = location[i];
    } else {
      _0 = 1;
      file->record_header.location_identifier[i] = ' ';
    }
  }
  file->record_header.location_identifier[2] = 0;
  /* Set channel_identifier. */
  for (i = 0, _0 = 0; i < 3; ++i) {
    if (!_0 && channel && channel[i]) {
      file->record_header.channel_identifier[i] = channel[i];
    } else {
      _0 = 1;
      file->record_header.channel_identifier[i] = ' ';
    }
  }
  file->record_header.channel_identifier[3] = 0;
  /* Set network_code. */
  for (i = 0, _0 = 0; i < 2; ++i) {
    if (!_0 && network && network[i]) {
      file->record_header.network_code[i] = network[i];
    } else {
      _0 = 1;
      file->record_header.network_code[i] = ' ';
    }
  }
  file->record_header.network_code[2] = 0;
  return 0;
}

int miniseed_file_set_compression(miniseed_file_t *file, int compression)
{
  if (!file) return -1;
  file->blockette_1000.encoding = compression ? 10 : 3;
  return 0;
}
