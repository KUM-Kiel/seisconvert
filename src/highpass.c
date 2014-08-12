#include <stdlib.h>
#include <stdio.h>
#include "wav.h"
#include "filter.h"

#define MAX_CHANNELS 16
#define STAGES 8

#define R(b, n) if (fread(b, n, 1, stdin) <= 0) return -1
#define W(b, n) if (fwrite(b, n, 1, stdout) <= 0) return -1

#define FOR(i, n) for (i = 0; i < n; ++i)

int main(int argc, char **argv)
{
  wav_header_t wh;
  wav_frame_config_t fc;
  uint8_t buffer[MAX_CHANNELS * 8];
  double frame[MAX_CHANNELS];
  biquad_filter_t filter[STAGES * MAX_CHANNELS];
  int i, j, k, cutoff;

  if (argc < 2) return -1;
  cutoff = atoi(argv[1]);
  if (cutoff <= 0) return -1;

  R(buffer, WAV_HEADER_BYTES);
  if (wav_header_read(&wh, buffer) < 0) {
    fprintf(stderr, "Invalid WAV header.\n");
    return -1;
  }
  fprintf(stderr, "WAV %uBit/%gkHz. Duration: %.2fs.\n", wh.bit_depth, wh.sample_rate / 1000.0, (double)wh.num_frames / wh.sample_rate);
  W(buffer, WAV_HEADER_BYTES);

  FOR(k, STAGES) {
    FOR(i, wh.num_channels) {
      biquad_filter_highpass(filter + i + MAX_CHANNELS * k, cutoff, wh.sample_rate);
    }
  }

  fc = wav_get_frame_config(&wh);

  FOR(i, wh.num_frames) {
    R(buffer, wav_get_frame_size(fc));
    wav_read_double_frame(fc, frame, buffer);
    FOR(k, STAGES) {
      FOR(j, wh.num_channels) {
        frame[j] = biquad_filter(filter + j + MAX_CHANNELS * k, frame[j]);
      }
    }
    wav_write_double_frame(fc, buffer, frame);
    W(buffer, wav_get_frame_size(fc));
  }

  return 0;
}
