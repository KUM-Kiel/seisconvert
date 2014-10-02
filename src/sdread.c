/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "kumy_file.h"
#include "number.h"
#include "taia.h"
#include "caltime.h"
#include "byte.h"

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
      printf("%08llx | ", (long long)i);
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

static void print_taia(const struct taia *t)
{
  struct caltime ct;
  unsigned int l;
  char s[30];
  caltime_utc(&ct, &t->sec, 0, 0);
  l = caltime_fmt(s, &ct);
  s[l] = 0;
  printf("%s", s);
}

static void kumy_binary_header_set_date(kumy_binary_header_t *bh, const struct taia *t)
{
  struct caltime ct;
  caltime_utc(&ct, &t->sec, 0, 0);
  bh->year = ct.date.year;
  bh->julian_day = julian_day(ct.date.day, ct.date.month, ct.date.year);
}

/* Prints a progress bar. If you intend to update the bar, set finished to 0. */
static void progress(int percent, int finished) {
  char s[29];
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  snprintf(s, 29, "[                    ] %3d%%%c", percent, finished ? '\n' : '\r');
  byte_set((uint8_t *)s + 1, percent / 5, '#');
  if (!fwrite(s, 28, 1, stdout)) return;
  fflush(stdout);
}

#define BLOCKSIZE 512
#define FRAMESIZE 16

static const int32_t lost_frame[] = {
  0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
};

#define e_io(c) do {\
  errorcode = c;\
  goto ioerror;\
} while (0)

#define e_fmt(c) do {\
  errorcode = c;\
  goto formaterror;\
} while (0)

int main(int argc, char **argv)
{
  FILE *sdcard = 0;
  kumy_file_t *kumy = 0;
  uint64_t addr, last_addr, writ, samp, i, j, n, frames = 0;
  uint8_t block[BLOCKSIZE];
  char filename[] = "KUM.recording.YYYY.DDD.HH.MM.SS.muk1";
  int32_t frame[4];
  int want_start_time = 1;
  int errorcode = 0;
  struct taia start_time;
  struct taia last_time;
  struct taia sync_time;
  struct taia skew_time;
  int synced = 0, skewed = 0;
  int percent = 0, old_percent = -1;
  int64_t sync_skew = 0, skew = 0;
  char message[2048];
  struct taia t;
  uint32_t lost = 0, lost_total = 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s /dev/sdx\n", argv[0]);
    return -1;
  }

  /* Open the SD card. */
  sdcard = fopen(argv[1], "rb");
  if (!sdcard) {
    fprintf(stderr, "Could not open file '%s': %s.\n", argv[1], strerror(errno));
    return -1;
  }

  /* Try reading the first block of the card. */
  if (!fread(block, BLOCKSIZE, 1, sdcard)) e_io(1);

  /* Check for constant fields in the first block. */
  if  (ld_u32_be(block)      != 0x74696d65 /* time */
    || ld_u32_be(block + 29) != 0x61646472 /* addr */
    || ld_u32_be(block + 37) != 0x73616d70 /* samp */
    || ld_u32_be(block + 43) != 0x77726974 /* writ */
    || ld_u32_be(block + 55) != 0x6e777269 /* nwri */
    || ld_u32_be(block + 63) != 0x6c6f7374 /* lost */
    || ld_u32_be(block + 71) != 0x74656d70 /* temp */
    || ld_u32_be(block + 77) != 0x68756d69 /* humi */
  ) e_fmt(2);

  /* Read start address and sample rate. */
  addr = ld_u32_be(block + 33);
  samp = ld_u16_be(block + 41);

  /* Read the synchronisation time and skew, if it is present. */
  if (byte_equal(block + 10, 9, (uint8_t *)"sync/skew")) {
    bcd_taia(&sync_time, block + 19);
    sync_skew = ld_i32_be(block + 25);
    synced = 1;
  }

  /* Read the second block. */
  if (!fread(block, BLOCKSIZE, 1, sdcard)) e_io(3);

  /* Check for constant fields in the second block. */
  if  (ld_u32_be(block)      != 0x74696d65 /* time */
    || ld_u32_be(block + 29) != 0x61646472 /* addr */
    || ld_u32_be(block + 37) != 0x73616d70 /* samp */
    || ld_u32_be(block + 43) != 0x77726974 /* writ */
    || ld_u32_be(block + 55) != 0x6e777269 /* nwri */
    || ld_u32_be(block + 63) != 0x6c6f7374 /* lost */
    || ld_u32_be(block + 71) != 0x74656d70 /* temp */
    || ld_u32_be(block + 77) != 0x68756d69 /* humi */
  ) e_fmt(4);

  /* Read end address and number of frames. */
  last_addr = ld_u32_be(block + 33);
  writ = ld_u64_be(block + 47);

  /* Read the skew time and skew, if it is present. */
  if (byte_equal(block + 10, 9, (uint8_t *)"sync/skew")) {
    bcd_taia(&skew_time, block + 19);
    skew = ld_i32_be(block + 25);
    skewed = 1;
  }

  /* Calculate the number of frames to read based on the block count. */
  n = (last_addr - addr) * (BLOCKSIZE / FRAMESIZE);

  /* Seek to the first data block. */
  if (fseek(sdcard, addr * BLOCKSIZE, SEEK_SET) == -1) e_io(5);

  /* Read n frames and process them. */
  for (i = 0; i < n; ++i) {
    /* Read a frame. */
    if (!fread(block, FRAMESIZE, 1, sdcard)) e_io(6);
    /* Load the first value to get the frame type. */
    frame[0] = ld_i32_be(block);
    /* If the last bit is 1, it is a control frame. Otherwise it is a normal
     * data frame. */
    if (!(frame[0] & 1) && !want_start_time) {
      ++frames;
      /* Load the other samples ... */
      frame[1] = ld_i32_be(block + 4);
      frame[2] = ld_i32_be(block + 8);
      frame[3] = ld_i32_be(block + 12);
      /* ... and write the frame to the outfile. */
      kumy_file_write_int_frame(kumy, frame);
    } else {
      switch (frame[0]) {
        case 1: /* Time control frame. */
          /* If this is the first time frame, it is the start time. */
          if (want_start_time) {
            /* Parse the time. */
            bcd_taia(&start_time, block + 4);
            last_time = start_time;
            want_start_time = 0;
            /* Create the outfile with start time in file name. */
            print_text_date((uint8_t *)filename + 14, &start_time);
            kumy = kumy_file_create(filename, samp);
            printf("%s\n", filename);
          } else {
            bcd_taia(&last_time, block + 4);
          }
          break;
        case 7: /* Lost frames. */
          bcd_taia(&t, block + 4);
          print_taia(&t);
          lost = ld_u32_be(block + 10);
          lost_total += lost;
          snprintf(message, sizeof(message), ": %lld Frames lost.", (long long)lost);
          printf("%s\n", message);
          for (j = 0; j < lost; ++j) {
            kumy_file_write_int_frame(kumy, lost_frame);
          }
          break;
        default: /* Other control frames. */
          break; /* Just ignore it. */
      }
    }
    /* Update the progress bar. */
    if ((i & 0x0fff) == 0 && !want_start_time) {
      percent = 100 * i / n;
      if (percent != old_percent) {
        progress(percent, 0);
        old_percent = percent;
      }
    }
  }
  progress(100, 1);

  /* Check for consistency with header. */
  (void) writ;
  /*if (writ != frames) {
    fprintf(stderr, "Warning: Number of frames read differs from number in header. %s\n", writ > frames ? "Some frames might be lost." : "There were extra frames.");
    fprintf(stderr, "%lld/%lld Frames. (%s%lld)\n", (long long)frames, (long long)writ, writ > frames ? "" : "+", (long long)(frames - writ));
  }*/

  /* Check if there was any data. */
  if (!want_start_time) {
    /* If there was no sync, assume the start time as sync time. */
    if (!synced) {
      sync_time = start_time;
    }
    /* If there was no skew measurement, assume the end time as skew time and
     * zero skew. */
    if (!skewed) {
      skew_time = last_time;
    }

    /* Fill out the remaining header fields. */
    for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
      print_text_date(kumy->text_header[i].content + 1871, &start_time);
      print_text_date(kumy->text_header[i].content + 1951, &last_time);
      print_text_date(kumy->text_header[i].content + 2031, &sync_time);
      print_text_date(kumy->text_header[i].content + 2111, &skew_time);
      write_int(kumy->text_header[i].content + 2049, 9, sync_skew, 1);
      kumy->text_header[i].content[2049] = sync_skew < 0 ? '-' : '+';
      write_int(kumy->text_header[i].content + 2129, 9, skew, 1);
      kumy->text_header[i].content[2129] = skew < 0 ? '-' : '+';
      write_int(kumy->text_header[i].content + 2191, 8, skew - sync_skew, 1);
      write_int(kumy->text_header[i].content + 2271, 8, 1000000 / samp, 1);
      write_int(kumy->text_header[i].content + 2351, 3, i + 1, 1);
      write_int(kumy->text_header[i].content + 2381, 3, KUMY_FILE_CHANNELS, 1);
      write_int(kumy->text_header[i].content + 2431, 3, 1, 1);
      kumy_binary_header_set_date(&kumy->binary_header[i], &start_time);
    }
    kumy_file_close(kumy);
  }

  fclose(sdcard);
  return 0;

ioerror:
  fclose(sdcard);
  fprintf(stderr, "IO error: There was an error while accessing the SD card. (Error %d)\n", errorcode);
  return -1;

formaterror:
  fclose(sdcard);
  fprintf(stderr, "Read error: The SD card is malformed. (Error %d)\n", errorcode);
  return -1;
}
