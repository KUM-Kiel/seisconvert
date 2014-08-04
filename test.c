#include <stdio.h>
#include "kum_segy.h"

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

int main(int argc, char **argv)
{
  uint8_t text_header[3200], binary_header[400];
  kum_segy_text_header_t kum_header_txt;
  kum_segy_binary_header_t kum_header_bin;
  if (argc < 2) return -1;
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Could not open file %s.\n", argv[1]);
    return -1;
  }
  for (int i = 0; i < 4; ++i) {
    fread(text_header, 3200, 1, f);
    fread(binary_header, 400, 1, f);
    kum_segy_text_header_read(&kum_header_txt, text_header);
    if (kum_segy_binary_header_read(&kum_header_bin, binary_header) == -1) {
      fprintf(stderr, "Someone has messed something up!\n");
    }
    print_kum_segy_txt(&kum_header_txt);
    print_kum_segy_bin(&kum_header_bin);
  }
  return 0;
}
