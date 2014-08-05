#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "wav.h"

int main(int argc, char **argv)
{
  int f, i, j, cutoff;
  double v, s, frame[2];
  wav_frame_config_t fc;
  wav_header_t hdr;
  uint8_t buffer[64];

  if (argc < 2)
    return -1;
  f = atoi(argv[1]);
  if (f <= 0)
    return -1;

  hdr.format = WAV_PCM;
  hdr.num_channels = 2;
  hdr.sample_rate = 44100;
  hdr.bit_depth = 16;
  hdr.num_frames = 30 * hdr.sample_rate;

  fc = wav_get_frame_config(&hdr);

  cutoff = hdr.sample_rate / 2;
  s = 1.0 / (cutoff / f);

  wav_header_write(buffer, &hdr);
  fwrite(buffer, WAV_HEADER_BYTES, 1, stdout);

  for (i = 0; i < hdr.num_frames; ++i) {
    v = 0;
    for (j = f; j <= cutoff; j += f) {
      v += cos(6.2831853072 * j * i / hdr.sample_rate);
    }
    v *= s;
    frame[0] = v;
    frame[1] = v;

    wav_write_double_frame(fc, buffer, frame);
    fwrite(buffer, wav_get_frame_size(fc), 1, stdout);
  }

  return 0;
}
