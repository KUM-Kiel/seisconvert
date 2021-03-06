/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include "kumy_file.h"
#include "wav_file.h"
#include "taia.h"
#include "caltime.h"
#include "number.h"
#include "byte.h"
#include "options.h"
#include "cli.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

static const char *program = "kumy2wav";
static void usage(const char *o, const char *x, int l)
{
  fprintf(stderr,
    "Usage: %s [options] infile.muk1\n"
    "Options:\n"
    "  -q, --quiet\n"
    "      Do not display a progress bar.\n"
    "  -h, --help\n"
    "      Show this help screen.\n"
    "\n",
    program);
  exit(1);
}

int parse_text_date(struct taia *t, const uint8_t *x)
{
  struct caltime ct;
  int64_t i;
  if (parse_int(&i, x,      4) < 0) return -1; ct.date.year = i;
  if (parse_int(&i, x + 5,  3) < 0) return -1; ct.date.day  = i;
  if (parse_int(&i, x + 9,  2) < 0) return -1; ct.hour      = i;
  if (parse_int(&i, x + 12, 2) < 0) return -1; ct.minute    = i;
  if (parse_int(&i, x + 15, 2) < 0) return -1; ct.second    = i;
  ct.date.month  = 1;
  ct.offset = 0;
  caltime_tai(&ct, &t->sec);
  t->nano = t->atto = 0;
  return 0;
}

static int last(int c, const char *x)
{
  int i, r = -1;
  for (i = 0; x[i] > 0; ++i) {
    if (x[i] == c) r = i;
  }
  return r;
}

static void copy(char *t, uint64_t l, const char *f)
{
  uint64_t i;
  for (i = 0; i < l; ++i) {
    t[i] = f[i];
  }
}

static int mkdir_p(const char *path)
{
  if (mkdir(path, 0755) < 0) {
    if (errno != EEXIST)
      return -1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  kumy_file_t *kumy;
  wav_file_t *wav[1];
  int32_t frame[KUMY_FILE_CHANNELS];
  uint64_t frames, l, frames_per_file, frames_total, frame_count = 0;
  int i;
  char oname[1024], folder[1024];
  uint32_t sample_rate, seconds_per_file;
  int percent = 0, old_percent = -1;
  int show_progress = 1;

  struct taia start_time; /* 1871 */
  struct taia stop_time;  /* 1951 */
  struct taia sync_time;  /* 2031 */
  struct taia skew_time;  /* 2111 */

  struct caltime ct;
  struct taia tt, dt;

  program = argv[0];
  parse_options(&argc, &argv, OPTIONS(
    FLAG('q', "quiet", show_progress, 0),
    FLAG_CALLBACK('h', "help", usage)
  ));

  if (argc < 2) usage(0, 0, 0);

  if (!(kumy = kumy_file_open(argv[1]))) {
    fprintf(stderr, "Invalid file: %s.\n", argv[1]);
    return -1;
  }

  parse_text_date(&start_time, kumy->text_header[0].content + 1871);
  parse_text_date(&stop_time,  kumy->text_header[0].content + 1951);
  parse_text_date(&sync_time,  kumy->text_header[0].content + 2031);
  parse_text_date(&skew_time,  kumy->text_header[0].content + 2111);

  sample_rate = 1000000 / kumy->binary_header[0].sample_interval;

  if (sample_rate <= 250) {
    seconds_per_file = 86400;
  } else if (sample_rate <= 500) {
    seconds_per_file = 43200;
  } else if (sample_rate <= 1000) {
    seconds_per_file = 21600;
  } else if (sample_rate <= 2000) {
    seconds_per_file = 10800;
  } else {
    seconds_per_file = 5400;
  }

  frames_per_file = sample_rate * seconds_per_file;
  frames_total = kumy->binary_header[0].num_samples;

  caltime_utc(&ct, &start_time.sec, 0, 0);
  ct.hour = 0;
  ct.minute = 0;
  ct.second = 0;
  caltime_tai(&ct, &tt.sec);
  tt.nano = tt.atto = 0;

  while (!taia_less(&start_time, &tt)) {
    tt.sec.x += seconds_per_file;
  }

  taia_sub(&dt, &tt, &start_time);
  frames = taia_approx(&dt) / seconds_per_file * frames_per_file;

  l = last('.', argv[1]);
  if (l == -1 || l >= sizeof(folder)) return -1;

  /* Create folder. */
  copy(folder, l, argv[1]);
  folder[l] = 0;
  mkdir_p(folder);

  for (i = 0; i < 1; ++i) {
    caltime_utc(&ct, &start_time.sec, 0, 0);
    snprintf(oname, sizeof(oname), "%s/%lld.%02lld.%02lld.%02lld.%02lld.%02lld.wav",
      folder, (long long)ct.date.year, (long long)ct.date.month, (long long)ct.date.day, (long long)ct.hour, (long long)ct.minute, (long long)ct.second);

    if (!(wav[i] = wav_file_create(oname, sample_rate, KUMY_FILE_CHANNELS, 32))) {
      fprintf(stderr, "Invalid file: %s.\n", oname);
      return -1;
    }
  }

  while (kumy_file_read_int_frame(kumy, frame) >= 0) {
    if (frames == 0) {
      /* Create new files.*/
      for (i = 0; i < 1; ++i) {
        wav_file_close(wav[i]);
        caltime_utc(&ct, &tt.sec, 0, 0);
        snprintf(oname, sizeof(oname), "%s/%lld.%02lld.%02lld.%02lld.%02lld.%02lld.wav",
          folder, (long long)ct.date.year, (long long)ct.date.month, (long long)ct.date.day, (long long)ct.hour, (long long)ct.minute, (long long)ct.second);
        if (!(wav[i] = wav_file_create(oname, sample_rate, KUMY_FILE_CHANNELS, 32))) {
          fprintf(stderr, "Invalid file: %s.\n", oname);
          return -1;
        }
      }
      frames = frames_per_file;
      tt.sec.x += seconds_per_file;
    }

    for (i = 0; i < 1; ++i) {
      wav_file_write_int_frame(wav[i], frame + i);
    }
    if (show_progress && frame_count % 10000 == 0) {
      percent = 100 * frame_count / frames_total;
      if (percent != old_percent) {
        progress(percent, 0);
        old_percent = percent;
      }
    }
    --frames;
    ++frame_count;
  }
  if (show_progress) progress(100, 1);

  kumy_file_close(kumy);
  for (i = 0; i < 1; ++i) {
    wav_file_close(wav[i]);
  }

  return 0;
}
