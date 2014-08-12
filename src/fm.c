#include <stdlib.h>
#include <stdio.h>
#include "wav.h"
#include "fm.h"

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
  fm_t fm[MAX_CHANNELS];
  int i, j, fg, fhub;

  if (argc < 3) return -1;
  fg = atoi(argv[1]);
  fhub = atoi(argv[2]);
  if (fg <= 0) return -1;
  if (fhub <= 0) return -1;

  R(buffer, WAV_HEADER_BYTES);
  if (wav_header_read(&wh, buffer) < 0) {
    fprintf(stderr, "Invalid WAV header.\n");
    return -1;
  }
  fprintf(stderr, "WAV %uBit/%gkHz. Duration: %.2fs.\n", wh.bit_depth, wh.sample_rate / 1000.0, (double)wh.num_frames / wh.sample_rate);
  W(buffer, WAV_HEADER_BYTES);

  FOR(i, wh.num_channels) {
    fm[i].f = fg;
    fm[i].hub = fhub;
    fm[i].phase = 0;
    fm[i].sample_interval = 1.0 / wh.sample_rate;
  }

  fc = wav_get_frame_config(&wh);

  FOR(i, wh.num_frames) {
    R(buffer, wav_get_frame_size(fc));
    wav_read_double_frame(fc, frame, buffer);
    FOR(j, wh.num_channels) {
      frame[j] = fm_modulate(fm + j, frame[j]);
    }
    wav_write_double_frame(fc, buffer, frame);
    W(buffer, wav_get_frame_size(fc));
  }

  return 0;
}
