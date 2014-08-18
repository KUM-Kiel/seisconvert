#include <stdlib.h>
#include <stdio.h>
#include "wav.h"

#define W(b, n) if (fwrite(b, n, 1, stdout) <= 0) return -1

int main(int argc, char **argv)
{
  wav_header_t wh;
  uint8_t h[WAV_HEADER_BYTES];
  int sample_rate, num_channels, bit_depth, num_frames;

  if (argc < 5) return -1;

  sample_rate = atoi(argv[1]);
  if (sample_rate <= 0) return -1;

  num_channels = atoi(argv[2]);
  if (num_channels <= 0) return -1;

  bit_depth = atoi(argv[3]);
  if (bit_depth <= 0) return -1;

  num_frames = atoi(argv[4]);
  if (num_frames <= 0) return -1;

  wh.format = WAV_PCM;
  wh.num_channels = num_channels;
  wh.sample_rate = sample_rate;
  wh.bit_depth = bit_depth;
  wh.num_frames = num_frames;

  fprintf(stderr, "WAV %uBit/%gkHz. Duration: %.2fs.\n", wh.bit_depth, wh.sample_rate / 1000.0, (double)wh.num_frames / wh.sample_rate);

  wav_header_write(h, &wh);
  W(h, WAV_HEADER_BYTES);
}
