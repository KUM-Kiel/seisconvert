/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "kumy_file.h"
#include "number.h"
#include "taia.h"
#include "caltime.h"

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

static int bcd_int(uint8_t bcd)
{
  return bcd / 16 * 10 + bcd % 16;
}

static void bcd_taia(struct taia *t, const uint8_t *bcd)
{
  struct caltime ct;
  ct.hour = bcd_int(bcd[0]);
  ct.minute = bcd_int(bcd[1]);
  ct.second = bcd_int(bcd[2]);
  ct.date.day = bcd_int(bcd[3]);
  ct.date.month = bcd_int(bcd[4]);
  ct.date.year = 2000 + bcd_int(bcd[5]);
  ct.offset = 0;
  caltime_tai(&ct, &t->sec);
  t->nano = 0;
  t->atto = 0;
}

static void print_text_date(uint8_t *x, const struct taia *t)
{
  struct caltime ct;
  caltime_utc(&ct, &t->sec, 0, 0);
  write_int(x     , 4, ct.date.year, 1);
  x[4] = '.';
  write_int(x +  5, 3, julian_day(ct.date.day, ct.date.month, ct.date.year), 1);
  x[8] = '.';
  write_int(x +  9, 2, ct.hour, 1);
  x[11] = '.';
  write_int(x + 12, 2, ct.minute, 1);
  x[14] = '.';
  write_int(x + 15, 2, ct.second, 1);
}

static void kumy_binary_header_set_date(kumy_binary_header_t *bh, const struct taia *t)
{
  struct caltime ct;
  caltime_utc(&ct, &t->sec, 0, 0);
  bh->year = ct.date.year;
  bh->julian_day = julian_day(ct.date.day, ct.date.month, ct.date.year);
}

#define BLOCKSIZE 512
#define FRAMESIZE 16

int main(int argc, char **argv)
{
  FILE *sdcard = 0;
  kumy_file_t *kumy = 0;
  uint64_t addr, writ, samp, i, frames = 0;
  uint8_t block[BLOCKSIZE];
  char filename[] = "KUM.recording.YYYY.DDD.HH.MM.SS.muk1";
  int32_t frame[4];
  int want_start_time = 1;
  struct taia start_time;
  struct taia last_time;

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
  addr = ld_u32_be(block + 33);
  samp = ld_u16_be(block + 41);

  if (!fread(block, BLOCKSIZE, 1, sdcard)) goto fail;
  writ = ld_u64_be(block + 47) * 896;

  printf("%lld %lld\n", (long long)addr, (long long)writ);

  if (fseek(sdcard, addr * BLOCKSIZE, SEEK_SET) == -1) goto fail;

  for (i = 0; i < writ; ++i) {
    if (!fread(block, FRAMESIZE, 1, sdcard)) goto fail;
    frame[0] = ld_i32_be(block);
    if (!(frame[0] & 1) && !want_start_time) {
      ++frames;
      frame[1] = ld_i32_be(block + 4);
      frame[2] = ld_i32_be(block + 8);
      frame[3] = ld_i32_be(block + 12);
      kumy_file_write_int_frame(kumy, frame);
    } else {
      if (frame[0] == 1) {
        /* Time */
        if (want_start_time) {
          bcd_taia(&start_time, block + 4);
          want_start_time = 0;
          /* Create file */
          print_text_date((uint8_t *)filename + 14, &start_time);
          kumy = kumy_file_create(filename, samp);
        } else {
          bcd_taia(&last_time, block + 4);
        }
      }
    }
  }
  printf("%d Frames.\n", (int)frames);

  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    print_text_date(kumy->text_header[i].content + 1871, &start_time);
    print_text_date(kumy->text_header[i].content + 1951, &last_time);
    print_text_date(kumy->text_header[i].content + 2031, &last_time);
    print_text_date(kumy->text_header[i].content + 2111, &last_time);
    write_int(kumy->text_header[i].content + 2191, 8, 0, 1);
    write_int(kumy->text_header[i].content + 2271, 8, 1000000 / samp, 1);
    write_int(kumy->text_header[i].content + 2351, 3, i, 1);
    write_int(kumy->text_header[i].content + 2431, 3, 1, 1);
    kumy_binary_header_set_date(&kumy->binary_header[i], &start_time);
  }

  kumy_file_close(kumy);
  fclose(sdcard);
  return 0;

fail:
  fclose(sdcard);
  fprintf(stderr, "Read error.\n");
  return -1;
}
