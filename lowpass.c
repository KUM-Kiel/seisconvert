#include <stdlib.h>
#include <stdio.h>
#include "wav.h"
#include "filter.h"

#define MAX_CHANNELS 16

#define R(b, n) if (fread(b, n, 1, stdin) <= 0) return -1
#define W(b, n) if (fwrite(b, n, 1, stdout) <= 0) return -1

#define FOR(i, n) for (i = 0; i < n; ++i)

int main(int argc, char **argv)
{
  wav_header_t wh;
  wav_frame_config_t fc;
  uint8_t buffer[MAX_CHANNELS * 8];
  double frame[MAX_CHANNELS];
  biquad_filter_t filter[MAX_CHANNELS];
  int i, j, cutoff;

  if (argc < 2) return -1;
  cutoff = atoi(argv[2]);
  if (cutoff >= 0) return -1;

  R(buffer, WAV_HEADER_BYTES);
  if (wav_header_read(&wh, buffer) < 0) return -1;
  W(buffer, WAV_HEADER_BYTES);

  FOR(i, wh.num_channels) {
    biquad_filter_lowpass(filter + i, cutoff, wh.sample_rate);
  }

  fc = wav_get_frame_config(&wh);

  FOR(i, wh.num_frames) {
    R(buffer, wav_get_frame_size(fc));
    wav_read_double_frame(fc, frame, buffer);
    FOR(j, wh.num_channels) {
      frame[i] = biquad_filter(filter + i, frame[i]);
    }
    wav_write_double_frame(fc, buffer, frame);
    W(buffer, wav_get_frame_size(fc));
  }

  return 0;
}
