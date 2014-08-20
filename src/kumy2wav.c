#include <stdlib.h>
#include <stdio.h>
#include "wav.h"
#include "kumy.h"

#define CHANNELS 4

#define R(b, n) if (fread(b, n, 1, stdin) <= 0) return -1
#define W(b, n) if (fwrite(b, n, 1, stdout) <= 0) return -1

#define FOR(i, n) for (i = 0; i < n; ++i)

int main(int argc, char **argv)
{
  wav_header_t wh;
  wav_frame_config_t fc;
  kumy_binary_header_t kum_header_bin;
  kumy_frame_config_t kfc;
  uint8_t text_header[3200], binary_header[400], buffer[64];
  int32_t frame[CHANNELS];
  int i, j;

  FOR(i, CHANNELS) {
    R(text_header, 3200);
    R(binary_header, 400);
    if (kumy_binary_header_read(&kum_header_bin, binary_header) == -1) {
      fprintf(stderr, "Someone has messed something up!\n");
      return -1;
    }
  }

  wh.format = WAV_PCM;
  wh.num_channels = CHANNELS;
  wh.sample_rate = 1000000 / kum_header_bin.sample_interval;
  wh.bit_depth = 32;
  wh.num_frames = kum_header_bin.num_samples;

  fprintf(stderr, "WAV %uBit/%gkHz. Duration: %.2fs.\n", wh.bit_depth, wh.sample_rate / 1000.0, (double)wh.num_frames / wh.sample_rate);

  wav_header_write(buffer, &wh);
  W(buffer, WAV_HEADER_BYTES);

  fc = wav_get_frame_config(&wh);
  kfc = kumy_get_frame_config(&kum_header_bin);

  FOR(i, wh.num_frames) {
    FOR(j, CHANNELS) {
      R(buffer, kumy_get_frame_size(kfc));
      kumy_read_int_frame(kfc, frame + j, buffer);
    }
    wav_write_int_frame(fc, buffer, frame);
    W(buffer, wav_get_frame_size(fc));
  }

  return 0;
}
