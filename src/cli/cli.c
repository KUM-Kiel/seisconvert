#include "cli.h"
#include <stdio.h>
#include <string.h>

/* Prints a progress bar. If you intend to update the bar, set finished to 0. */
void progress(int percent, int finished) {
  char bar[] = "                    ";
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  memset(bar, '#', percent / 5);
  fprintf(stdout, "\033[K[%s] %3d%%%c", bar, percent, finished ? '\n' : '\r');
  fflush(stdout);
}
