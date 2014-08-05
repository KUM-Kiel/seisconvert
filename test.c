#include <stdio.h>
#include "kum_segy.h"
#include "wav.h"

#define FOR(i, n) for (i = 0; i < n; ++i)

static void print_kum_segy_txt(kum_segy_text_header_t *header)
{
  printf("SEG-Y Text Header\n");
  printf("  client:          %s\n", header->client);
  printf("  company:         %s\n", header->company);
  printf("  crew_no:         %s\n", header->crew_no);
  printf("  line:            %s\n", header->line);
  printf("  area:            %s\n", header->area);
  printf("  reel_day_start:  %s\n", header->reel_day_start);
  printf("  reel_year:       %s\n", header->reel_year);
  printf("  observer:        %s\n", header->observer);
  printf("  instrument:      %s\n", header->instrument);
  printf("  model:           %s\n", header->model);
  printf("  serial_no:       %s\n", header->serial_no);
}

static void print_kum_segy_bin(kum_segy_binary_header_t *header)
{
  printf("SEG-Y Binary Header\n");
  printf("  job_id:          %x\n", header->job_id);
  printf("  year:            %u\n", header->year);
  printf("  julian_day:      %u\n", header->julian_day);
  printf("  recorder_no:     %u\n", header->recorder_no);
  printf("  channel_no:      %u\n", header->channel_no);
  printf("  sample_interval: %u\n", header->sample_interval);
  printf("  num_samples:     %u\n", header->num_samples);
}

#define CHANNELS 4

int main(int argc, char **argv)
{
  uint8_t text_header[3200], binary_header[400], buffer[64];
  int32_t frame[CHANNELS];
  kum_segy_text_header_t kum_header_txt;
  kum_segy_binary_header_t kum_header_bin;
  kum_segy_frame_config_t kfc;
  wav_frame_config_t wfc;
  wav_header_t whdr;
  int i, j, messed = 0;

  if (argc < 3) return -1;
  FILE *f = fopen(argv[1], "r");
  FILE *wav = fopen(argv[2], "w");
  if (!f || !wav) {
    fprintf(stderr, "Could not open file %s.\n", argv[1]);
    return -1;
  }
  FOR(i, CHANNELS) {
    fread(text_header, 3200, 1, f);
    fread(binary_header, 400, 1, f);
    kum_segy_text_header_read(&kum_header_txt, text_header);
    if (kum_segy_binary_header_read(&kum_header_bin, binary_header) == -1) {
      fprintf(stderr, "Someone has messed something up!\n");
      messed = 1;
    }
    print_kum_segy_txt(&kum_header_txt);
    print_kum_segy_bin(&kum_header_bin);
  }

  if (messed) return -1;

  whdr.format = WAV_PCM;
  whdr.num_channels = CHANNELS;
  whdr.sample_rate = 1000000 / kum_header_bin.sample_interval;
  whdr.bit_depth = 16;
  whdr.num_frames = kum_header_bin.num_samples;

  wfc = wav_get_frame_config(&whdr);
  kfc = kum_segy_get_frame_config(&kum_header_bin);

  wav_header_write(buffer, &whdr);
  fwrite(buffer, WAV_HEADER_BYTES, 1, wav);

  FOR(i, whdr.num_frames) {
    FOR(j, CHANNELS) {
      fread(buffer, kum_segy_get_frame_size(kfc), 1, f);
      kum_segy_read_int_frame(kfc, frame + j, buffer);
    }
    wav_write_int_frame(wfc, buffer, frame);
    fwrite(buffer, wav_get_frame_size(wfc), 1, wav);
  }

  return 0;
}
