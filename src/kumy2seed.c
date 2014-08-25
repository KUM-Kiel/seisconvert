#include <stdio.h>
#include <stdlib.h>
#include "kumy_file.h"
#include "miniseed_file.h"
#include "taia.h"
#include "caltime.h"
#include "number.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

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

static const char channel_names[KUMY_FILE_CHANNELS] = "HXYZ";
static const char ending[] = ".C.seed";

int main(int argc, char **argv)
{
  kumy_file_t *kumy;
  miniseed_file_t *mseed[KUMY_FILE_CHANNELS];
  int32_t frame[KUMY_FILE_CHANNELS];
  uint64_t frames, l, frames_per_file;
  int i;
  char oname[1024], folder[1024];
  uint32_t sample_rate, seconds_per_file;

  struct taia start_time; /* 1871 */
  struct taia stop_time;  /* 1951 */
  struct taia sync_time;  /* 2031 */
  struct taia skew_time;  /* 2111 */

  struct caltime ct;
  struct taia tt, dt;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <infile.muk1>\n", argv[0]);
    return -1;
  }

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

  /*printf("%016llx.%08lx.%08lx\n", start_time.sec.x, start_time.nano, start_time.atto);
  printf("%016llx.%08lx.%08lx\n", stop_time.sec.x, stop_time.nano, stop_time.atto);
  printf("%016llx.%08lx.%08lx\n", sync_time.sec.x, sync_time.nano, sync_time.atto);
  printf("%016llx.%08lx.%08lx\n", skew_time.sec.x, skew_time.nano, skew_time.atto);*/

  l = last('.', argv[1]);
  if (l == -1 || l >= sizeof(folder)) return -1;

  /* Create folder. */
  copy(folder, l, argv[1]);
  folder[l] = 0;
  mkdir_p(folder);

  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    caltime_utc(&ct, &start_time.sec, 0, 0);
    snprintf(oname, sizeof(oname), "%s/%ld.%02d.%02d.%02d.%02d.%02d.%c.seed",
      folder, ct.date.year, ct.date.month, ct.date.day, ct.hour, ct.minute, ct.second, channel_names[i]);

    if (!(mseed[i] = miniseed_file_create((char*)oname))) {
      fprintf(stderr, "Invalid file: %s.\n", oname);
      return -1;
    }
    miniseed_file_set_sample_rate(mseed[i], sample_rate);
    miniseed_file_set_start_time(mseed[i], &start_time);
    mseed[i]->record_header.channel_identifier[0] = channel_names[i];
  }

  while (kumy_file_read_int_frame(kumy, frame) >= 0) {
    if (frames == 0) {
      /* Create new files.*/
      for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
        miniseed_file_close(mseed[i]);
        caltime_utc(&ct, &tt.sec, 0, 0);
        snprintf(oname, sizeof(oname), "%s/%ld.%02d.%02d.%02d.%02d.%02d.%c.seed",
          folder, ct.date.year, ct.date.month, ct.date.day, ct.hour, ct.minute, ct.second, channel_names[i]);
        if (!(mseed[i] = miniseed_file_create((char*)oname))) {
          fprintf(stderr, "Invalid file: %s.\n", oname);
          return -1;
        }
        miniseed_file_set_sample_rate(mseed[i], sample_rate);
        miniseed_file_set_start_time(mseed[i], &tt);
        mseed[i]->record_header.channel_identifier[0] = channel_names[i];
      }
      frames = frames_per_file;
      tt.sec.x += seconds_per_file;
    }

    for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
      miniseed_file_write_int_frame(mseed[i], frame + i);
    }
    --frames;
  }

  kumy_file_close(kumy);
  for (i = 0; i < KUMY_FILE_CHANNELS; ++i) {
    miniseed_file_close(mseed[i]);
  }

  return 0;
}
