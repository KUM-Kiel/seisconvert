#include <stdio.h>
#include <stdlib.h>
#include "miniseed_file.h"
#include "wav_file.h"
#include "caltime.h"

int main(int argc, char **argv)
{
  miniseed_file_t *mseed;
  wav_file_t *wav;
  int32_t frame[16];
  uint64_t frames = 0;
  struct caltime ct;
  struct taia start_time;

  if (argc < 4) {
    fprintf(stderr, "Usage: %s <start time> <infile.wav> <outfile.seed>\n", argv[0]);
    return -1;
  }

  if (!(caltime_scan(argv[1], &ct))) {
    fprintf(stderr, "Malformed start time.\n");
    return -1;
  }
  caltime_tai(&ct, &start_time.sec);
  start_time.nano = 0;
  start_time.atto = 0;

  if (!(wav = wav_file_open(argv[2]))) {
    fprintf(stderr, "Invalid file: %s.\n", argv[2]);
    return -1;
  }

  if (!(mseed = miniseed_file_create(argv[3]))) {
    fprintf(stderr, "Invalid file: %s.\n", argv[3]);
    return -1;
  }
  miniseed_file_set_sample_rate(mseed, wav->header.sample_rate);
  miniseed_file_set_start_time(mseed, &start_time);

  while (wav_file_read_int_frame(wav, frame) >= 0) {
    miniseed_file_write_int_frame(mseed, frame);
    ++frames;
  }

  wav_file_close(wav);
  miniseed_file_close(mseed);

  return 0;
}
