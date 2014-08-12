#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "wav.h"

#define CHANNELS 2

#define R(b, n) if (fread(b, n, 1, stdin) <= 0) return -1
#define W(b, n) if (fwrite(b, n, 1, stdout) <= 0) return -1

#define FOR(i, n) for (i = 0; i < n; ++i)

int main(int argc, char **argv)
{
  wav_header_t wh;
  wav_frame_config_t fc;
  uint8_t buffer[64];
  double frame[CHANNELS], v;
  int i, j, f;

  if (argc < 2) return -1;
  f = atoi(argv[1]);
  if (f <= 0) return -1;

  wh.format = WAV_PCM;
  wh.num_channels = CHANNELS;
  wh.sample_rate = 44100;
  wh.bit_depth = 16;
  wh.num_frames = 30 * wh.sample_rate;

  fprintf(stderr, "WAV %uBit/%gkHz. Duration: %.2fs.\n", wh.bit_depth, wh.sample_rate / 1000.0, (double)wh.num_frames / wh.sample_rate);

  W(buffer, wav_header_write(buffer, &wh));

  fc = wav_get_frame_config(&wh);

  FOR(i, wh.num_frames) {
    v = cos(6.2831853072 * i * f / wh.sample_rate);
    FOR(j, wh.num_channels) {
      frame[j] = v;
    }
    W(buffer, wav_write_double_frame(fc, buffer, frame));
  }

  return 0;
}
