#include <math.h>
#include <stdlib.h>
#include "wav_file.h"

#define CHANNELS 2
#define SAMPLE_RATE 192000

#define FOR(i, n) for (i = 0; i < n; ++i)

int main(int argc, char **argv)
{
  wav_file_t *wav;
  double frame[CHANNELS], v;
  int i, j, f;

  if (argc < 3) return -1;

  f = atoi(argv[1]);
  if (f <= 0) return -1;

  wav = wav_file_create(argv[2], SAMPLE_RATE, CHANNELS, 32);
  if (!wav) return -1;

  FOR(i, 30 * SAMPLE_RATE) {
    v = cos(6.2831853072 * i * f / SAMPLE_RATE);
    FOR(j, CHANNELS) {
      frame[j] = v;
    }
    wav_file_write_double_frame(wav, frame);
  }
  wav_file_close(wav);
  return 0;
}
