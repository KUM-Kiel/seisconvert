/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "kumy_file.h"
#include "number.h"
#include "taia.h"
#include "caltime.h"
#include "byte.h"
#include "bcd.h"
#include "options.h"

/* Converts a BCD encoded date to taia. */
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

/* Formats a taia time for use in the textual header. (YYYY.DDD.HH.MM.SS) */
static int print_text_date(uint8_t *x, const struct taia *t)
{
  struct caltime ct;
  int yday;
  caltime_utc(&ct, &t->sec, 0, &yday);
  write_int(x     , 4, ct.date.year, 1);
  x[4] = '.';
  write_int(x +  5, 3, yday, 1);
  x[8] = '.';
  write_int(x +  9, 2, ct.hour, 1);
  x[11] = '.';
  write_int(x + 12, 2, ct.minute, 1);
  x[14] = '.';
  write_int(x + 15, 2, ct.second, 1);
  return 17;
}

/* Formats a taia-encoded time as UTC.
 * Returns the number of characters written. */
static int write_taia(const struct taia *t, char *s)
{
  struct caltime ct;
  caltime_utc(&ct, &t->sec, 0, 0);
  return caltime_fmt(s, &ct);
}

/* Shortcut for printing a taia encoded time directly to a file. */
static void print_taia(const struct taia *t, FILE *f)
{
  char s[32];
  s[write_taia(t, s)] = 0;
  fprintf(f, "%s", s);
}

/* Puts the given taia time into the binary header. */
static void kumy_binary_header_set_date(kumy_binary_header_t *bh, const struct taia *t)
{
  struct caltime ct;
  int yday;
  caltime_utc(&ct, &t->sec, 0, &yday);
  bh->year = ct.date.year;
  bh->julian_day = yday;
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

#define isalphanum(c) (('0' <= (c) && (c) <= '9') || \
  ('A' <= (c) && (c) <= 'Z') || ('a' <= (c) && (c) <= 'z'))

/* Replaces all special characters in a string with an underscore. */
static void string_safe(char *dst, const char *src)
{
  while (*src) {
    if (isalphanum(*src))
      *dst = *src;
    else
      *dst = '_';
    ++src;
    ++dst;
  }
  *dst = 0;
}

#define max(a, b) ((a) < (b) ? (b) : (a))
#define min(a, b) ((a) > (b) ? (b) : (a))

#define BLOCKSIZE 512
#define FRAMESIZE 16

/* Frame, that will be written to the file if a frame has been lost. */
static const int32_t lost_frame[] = {
  0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff
};

/* Macros for signalling an error. */
/* IO error */
#define e_io(c) do {\
  errorcode = c;\
  goto ioerror;\
} while (0)

/* Format error */
#define e_fmt(c) do {\
  errorcode = c;\
  goto formaterror;\
} while (0)

/* Some string constants. */
static const char
  *filename_template = "KUM.%s.%s.%s",
  *muk_extension = "muk1",
  *log_extension = "log.txt",
  *default_comment = "Recording";

#define WANT_START_TIME() goto end

static char *program = "sdread";
static void usage(const char *o, const char *x, int l)
{
  fprintf(stderr, "Usage: %s /dev/sdx\n", program);
  exit(-1);
}

int main(int argc, char **argv)
{
  FILE *sdcard = 0, *logfile = 0, *controlframes = 0;
  FILE *voltage_csv = 0, *temperature_csv = 0, *humidity_csv = 0;
  kumy_file_t *kumy = 0;
  uint64_t addr, last_addr, writ = 0, samp = 1, temp, humi, gain[4], i, j, k, m = 0, n, frames = 0;
  char comment[41], safe_comment[41];
  uint8_t block[BLOCKSIZE];
  char filename[256], tmp[64];
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
  char message[4096] = {0};
  int message_n = 0;
  struct taia t;
  uint64_t lost = 0, lost_total = 0;
  uint8_t control[6];
  int have_control_block = 0;
  int flag_debug = 0;

  program = argv[0];
  parse_options(&argc, &argv, OPTIONS(
    FLAG('d', "debug", flag_debug, 1),
    FLAG_CALLBACK('h', "help", usage)
  ));

  /* Check for SD card argument. */
  if (argc < 2) {
    usage(0, 0, 0);
  }

  /* Open the SD card. */
  sdcard = fopen(argv[1], "rb");
  if (!sdcard) {
    fprintf(stderr, "Could not open file '%s': %s.\n", argv[1], strerror(errno));
    return -1;
  }

  if (flag_debug)
    controlframes = fopen("controlframes.debug.txt", "wb");

  /* Try reading the first block of the card. */
  if (!fread(block, BLOCKSIZE, 1, sdcard)) e_io(1);

  /* Check for constant fields in the first block. */
  if  (ld_u32_be(block)      != 0x74696d65ul /* time */
    || ld_u32_be(block + 29) != 0x61646472ul /* addr */
    || ld_u32_be(block + 37) != 0x73616d70ul /* samp */
    || ld_u32_be(block + 43) != 0x77726974ul /* writ */
    || ld_u32_be(block + 55) != 0x6e777269ul /* nwri */
    || ld_u32_be(block + 63) != 0x6c6f7374ul /* lost */
    || ld_u32_be(block + 71) != 0x74656d70ul /* temp */
    || ld_u32_be(block + 77) != 0x68756d69ul /* humi */
    || ld_u32_be(block + 83) != 0x6761696eul /* gain */
    || ld_u32_be(block + 91) != 0x636d6e74ul /* cmnt */
  ) e_fmt(2);

  /* Read start address and sample rate. */
  addr = ld_u32_be(block + 33);
  samp = ld_u16_be(block + 41);
  temp = ld_u16_be(block + 75); (void)temp;
  humi = ld_u16_be(block + 81); (void)humi;
  gain[0] = block[87];
  gain[1] = block[88];
  gain[2] = block[89];
  gain[3] = block[90];
  byte_copy_0((uint8_t *)comment, 0, block + 95);
  string_safe(safe_comment, comment);

  /* Read the synchronisation time and skew, if it is present. */
  if (byte_equal(block + 10, 9, (uint8_t *)"sync/skew") &&
    bcd_valid((char *)block + 19)) {
    bcd_taia(&sync_time, block + 19);
    sync_skew = ld_i32_be(block + 25);
    synced = 1;
  }

  /* Read the second block. */
  if (!fread(block, BLOCKSIZE, 1, sdcard)) e_io(3);

  /* Check for constant fields in the second block. */
  if  (ld_u32_be(block)      != 0x74696d65ul /* time */
    || ld_u32_be(block + 29) != 0x61646472ul /* addr */
    || ld_u32_be(block + 37) != 0x73616d70ul /* samp */
    || ld_u32_be(block + 43) != 0x77726974ul /* writ */
    || ld_u32_be(block + 55) != 0x6e777269ul /* nwri */
    || ld_u32_be(block + 63) != 0x6c6f7374ul /* lost */
    || ld_u32_be(block + 71) != 0x74656d70ul /* temp */
    || ld_u32_be(block + 77) != 0x68756d69ul /* humi */
    || ld_u32_be(block + 83) != 0x6761696eul /* gain */
    || ld_u32_be(block + 91) != 0x636d6e74ul /* cmnt */
  ) e_fmt(4);

  /* Read end address and number of frames. */
  last_addr = ld_u32_be(block + 33);
  writ = ld_u64_be(block + 47);

  /* Read the skew time and skew, if it is present. */
  if (byte_equal(block + 10, 9, (uint8_t *)"sync/skew") &&
    bcd_valid((char *)block + 19)) {
    bcd_taia(&skew_time, block + 19);
    skew = ld_i32_be(block + 25);
    skewed = 1;
  }

  message_n += snprintf(message + message_n, max(sizeof(message) - message_n, 0),
    "# Recording\n\n"
    "Comment: %s\n"
    "Frames: %lld\n"
    "Samplerate: %lld Hz\n"
    "Channel Gains: H %.1f, X %.1f, Y %.1f, Z %.1f\n",
    comment,
    (long long)writ,
    (long long)samp,
    gain[0] / 10.0, gain[1] / 10.0, gain[2] / 10.0, gain[3] / 10.0);
  if (synced) {
    tmp[write_taia(&sync_time, tmp)] = 0;
    message_n += snprintf(message + message_n, max(sizeof(message) - message_n, 0),
      "Synchronisation Time: %s\n"
      "Skew at Synchronisation: %lld us\n",
      tmp,
      (long long)sync_skew);
  }
  if (skewed) {
    tmp[write_taia(&skew_time, tmp)] = 0;
    message_n += snprintf(message + message_n, max(sizeof(message) - message_n, 0),
      "Skew Measurement Time: %s\n"
      "Skew at Skew Measurement: %lld us\n",
      tmp,
      (long long)skew);
  }
  message_n += snprintf(message + message_n, max(sizeof(message) - message_n, 0),
    "\n# Eventlog\n\n");
  /* Calculate the number of frames to read based on the block count. */
  n = (last_addr - addr) * (BLOCKSIZE / FRAMESIZE);

  /* Seek to the first data block. */
  if (fseek(sdcard, addr * BLOCKSIZE, SEEK_SET) == -1) e_io(5);

  /* Read n frames and process them. */
  for (i = 0; i < n; ++i) {
    /* Read a frame. */
    if (!fread(block, FRAMESIZE, 1, sdcard)) {
      printf("Read Error.\n");
      fflush(0);
      exit(-3);
      if (!want_start_time) {
        if (logfile) {
          print_taia(&last_time, logfile);
          fprintf(logfile, ": Bad frame.\n");
        }
        for (j = 0; j < 100 * BLOCKSIZE / FRAMESIZE; ++j) {
          kumy_file_write_int_frame(kumy, lost_frame);
          ++i;
        }
        if (fseek(sdcard, 100 * BLOCKSIZE, SEEK_CUR) == -1) e_io(6);
      }
      continue;
    }
    /* Load the first value to get the frame type. */
    frame[0] = ld_i32_be(block);
    /* If the last bit is 1, it is a control frame. Otherwise it is a normal
     * data frame. */
    if ((!(frame[0] & 1) || frame[0] < 0) && !want_start_time) {
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
          /* Process only valid frames. */
          if (!bcd_valid((char *)block + 4)) break;
          /* If this is the first time frame, it is the start time. */
          if (want_start_time) {
            /* Parse the time. */
            bcd_taia(&start_time, block + 4);
            last_time = start_time;
            want_start_time = 0;
            /* Create the outfile with start time in file name. */
            tmp[print_text_date((uint8_t *)tmp, &start_time)] = 0;
            snprintf(filename, sizeof(filename), filename_template,
              *safe_comment ? safe_comment : default_comment, tmp, muk_extension);
            kumy = kumy_file_create(filename, samp);
            printf("%s\n", filename);
            /* Start logfile. */
            snprintf(filename, sizeof(filename), filename_template,
              *safe_comment ? safe_comment : default_comment, tmp, log_extension);
            logfile = fopen(filename, "w");
            if (logfile) {
              fprintf(logfile, "%s", message);
              print_taia(&start_time, logfile);
              fprintf(logfile, ": Recording started.\n");
            }
            /* Open CSV files. */
            snprintf(filename, sizeof(filename), filename_template,
              *safe_comment ? safe_comment : default_comment, tmp, "voltage.csv");
            voltage_csv = fopen(filename, "w");

            snprintf(filename, sizeof(filename), filename_template,
              *safe_comment ? safe_comment : default_comment, tmp, "temperature.csv");
            temperature_csv = fopen(filename, "w");

            snprintf(filename, sizeof(filename), filename_template,
              *safe_comment ? safe_comment : default_comment, tmp, "humidity.csv");
            humidity_csv = fopen(filename, "w");

            if (controlframes) {
              print_taia(&start_time, controlframes);
              putc('\n', controlframes);
            }
          } else {
            bcd_taia(&t, block + 4);
            if (controlframes) {
              print_taia(&t, controlframes);
              fprintf(controlframes, " (%lld)\n", (long long)(frames - m));
            }
            m = frames;
            if (taia_less(&last_time, &t)) {
              last_time = t;
            }
          }
          break;
        case 3: /* VBat/Humidity */
          if (controlframes) {
            fprintf(controlframes, "Battery Voltage: %.2f V.\n",
              ld_u16_be(block + 4) * 0.01);
            fprintf(controlframes, "Humidity: %.2f%%.\n",
              ld_u16_be(block + 6) * 0.01);
          }
          if (want_start_time) WANT_START_TIME();
          if (voltage_csv) {
            fprintf(voltage_csv, "%lld;%.2f\n",
              (long long)(tai_gps_sec(&last_time.sec)),
              ld_u16_be(block + 4) * 0.01);
          }
          if (humidity_csv) {
            fprintf(humidity_csv, "%lld;%.2f\n",
              (long long)(tai_gps_sec(&last_time.sec)),
              ld_u16_be(block + 6) * 0.01);
          }
          break;
        case 5: /* Temperature */
          if (controlframes) {
            fprintf(controlframes, "Temperature: %.2f °C.\n",
              ld_u16_be(block + 4) * 0.01);
          }
          if (want_start_time) WANT_START_TIME();
          if (temperature_csv) {
            fprintf(temperature_csv, "%lld;%.2f\n",
              (long long)(tai_gps_sec(&last_time.sec)),
              ld_u16_be(block + 4) * 0.01);
          }
          break;
        case 7: /* Lost frames. */
          if (want_start_time) WANT_START_TIME();
          bcd_taia(&t, block + 4);
          lost = ld_u32_be(block + 10);
          lost_total += lost;
          if (logfile) {
            print_taia(&t, logfile);
            fprintf(logfile, ": %lld Frames lost.\n", (long long)lost);
          }
          for (j = 0; j < min(lost, 65536); ++j) {
            kumy_file_write_int_frame(kumy, lost_frame);
          }
          break;
        case 9: /* Control frame */
          if (want_start_time) WANT_START_TIME();
          if (have_control_block) {
            if (!byte_equal(control, 6, block + 4)) {
              if (logfile) {
                print_taia(&last_time, logfile);
                fprintf(logfile, ": Block check failed.\n");
              }
              goto end;
            }
          } else {
            byte_copy(control, 6, block + 4);
            have_control_block = 1;
            if (logfile) {
              print_taia(&last_time, logfile);
              fprintf(logfile, ": Control block '%02x %02x %02x %02x %02x %02x'.\n",
                (int)control[0], (int)control[1], (int)control[2],
                (int)control[3], (int)control[4], (int)control[5]);
            }
          }
          break;
        case 11: /* Reboot */
          break;
        case 13: /* End */
          goto end;
          break;
        case 15: /* Written Frames */
          k = ld_u64_be(block + 4);
          if (controlframes) {
            fprintf(controlframes, "Frame %lld.\n", (long long)k);
          }
          if (want_start_time) WANT_START_TIME();
          if (frames < k) {
            /* Frames lost */
            if (logfile) {
              print_taia(&last_time, logfile);
              fprintf(logfile, ": %lld Frames lost.\n", (long long)(k - frames));
            }
            while (frames < k) {
              kumy_file_write_int_frame(kumy, lost_frame);
              ++frames;
            }
          } else if (frames > k) {
            /* Too much frames */
            if (logfile) {
              print_taia(&last_time, logfile);
              fprintf(logfile, ": %lld Frames too much.\n", (long long)(frames - k));
            }
            frames = k;
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
      fflush(0);
    }
  }
end:
  progress(100, 1);

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

    if (logfile) {
      print_taia(&last_time, logfile);
      fprintf(logfile, ": Recording ended.\n");

      /* Check for consistency with header. */
      if (writ != frames) {
        fprintf(logfile,
          "\nWarning: Number of frames read differs from number in header. %s\n"
          "%lld/%lld Frames. (%+lld)\n",
          writ > frames ? "Some frames might be lost." : "There were extra frames.",
          (long long)frames, (long long)writ,
          (long long)(frames - writ));
      }
    }

    kumy_file_close(kumy);
    if (logfile) fclose(logfile);
    if (voltage_csv) fclose(voltage_csv);
    if (temperature_csv) fclose(temperature_csv);
    if (humidity_csv) fclose(humidity_csv);
  }

  if (controlframes) fclose(controlframes);

  fclose(sdcard);
  return 0;

ioerror:
  fprintf(stderr, "IO error: There was an error while accessing the SD card."
    " (Error %d)\n", errorcode);
  goto end;
  return -1;

formaterror:
  fclose(sdcard);
  fprintf(stderr, "Read error: The SD card content is malformed."
    " (Error %d)\n", errorcode);
  return -1;
}
