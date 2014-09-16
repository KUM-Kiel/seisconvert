/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "wav_file.h"
#include "number.h"

typedef uint8_t u8;
typedef uint64_t u64;

static u8 printable(u8 c)
{
  if (32 <= c && c <= 126)
    return c;
  else
    return '_';
}

void hexdump(const u8 *b, u64 l)
{
  u64 i, j;
  u8 s[17];
  for (i = 0; i < 17; ++i) s[i] = 0;
  for (i = 0; i < l; i += 1) {
    if (i > 0) {
      if (i % 16 == 0) {
        printf(" | %s\n", s);
        for (j = 0; j < 17; ++j) s[j] = 0;
      } else if (i % 4 == 0) {
        printf("  ");
      } else {
        printf(" ");
      }
    }
    if (i % 16 == 0)
      printf("%08llx | ", i);
    printf("%02x", b[i]);
    s[i % 16] = printable(b[i]);
  }
  while (i % 16 != 0) {
    if (i % 4 == 0)
      printf("    ");
    else
      printf("   ");
    ++i;
  }
  printf(" | %s\n", s);
}

#define BLOCKSIZE 512
#define FRAMESIZE 16

int main(int argc, char **argv)
{
  FILE *sdcard;
  wav_file_t *wav;
  uint64_t addr, last_addr, writ, samp, i, frames = 0;
  uint8_t block[BLOCKSIZE];
  int32_t frame[4];

  if (argc < 2) {
    fprintf(stderr, "Usage: %s /dev/sdx\n", argv[0]);
    return -1;
  }

  sdcard = fopen(argv[1], "rb");
  if (!sdcard) {
    fprintf(stderr, "Could not open file '%s': %s.\n", argv[1], strerror(errno));
    return -1;
  }

  if (!fread(block, BLOCKSIZE, 1, sdcard)) goto fail;
  addr = ld_u32_le(block + 33);
  samp = ld_u16_be(block + 41);

  if (!fread(block, BLOCKSIZE, 1, sdcard)) goto fail;
  last_addr = ld_u32_le(block + 33);
  writ = ld_u32_be(block + 47) * 896;

  printf("%d %d %d %d\n", (int)addr, (int)last_addr, (int)writ, (int)samp);

  wav = wav_file_create("test.wav", samp, 4, 32);

  fseek(sdcard, addr * BLOCKSIZE, SEEK_SET);

  for (i = 0; i < writ; ++i) {
    if (!fread(block, FRAMESIZE, 1, sdcard)) goto fail;
    frame[0] = ld_i32_be(block);
    if (!(frame[0] & 1)) {
      ++frames;
      frame[1] = ld_i32_be(block + 4);
      frame[2] = ld_i32_be(block + 8);
      frame[3] = ld_i32_be(block + 12);
      wav_file_write_int_frame(wav, frame);
    }
  }
  printf("%d Frames.\n", (int)frames);

  wav_file_close(wav);
  fclose(sdcard);
  return 0;

fail:
  fclose(sdcard);
  fprintf(stderr, "Read error.\n");
  return -1;
}
