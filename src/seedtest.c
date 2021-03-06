#include <stdio.h>
#include <stdlib.h>
#include "miniseed_file.h"
#include "wav_file.h"

int main(int argc, char **argv)
{
  miniseed_file_t *mseed;
  wav_file_t *wav;
  int sample_rate;
  int32_t frame[1];
  uint64_t frames = 0;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <infile.seed> <outfile.wav>\n", argv[0]);
    return -1;
  }

  if (!(mseed = miniseed_file_open(argv[1]))) {
    fprintf(stderr, "Invalid file: %s.\n", argv[1]);
  }

  sample_rate = seed_sample_rate(
    mseed->record_header.sample_rate_factor,
    mseed->record_header.sample_rate_multiplier);
  if (!sample_rate) sample_rate = 1;

  if (!(wav = wav_file_create(argv[2], sample_rate, 1, 32))) {
    fprintf(stderr, "Invalid file: %s.\n", argv[2]);
  }

  while (miniseed_file_read_int_frame(mseed, frame) >= 0) {
    wav_file_write_int_frame(wav, frame);
    ++frames;
  }

  miniseed_file_close(mseed);
  wav_file_close(wav);

  fprintf(stderr, "%lld frames.\n", (long long)frames);

  return 0;
}
