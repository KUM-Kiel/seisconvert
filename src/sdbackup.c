/* Needed for files larger than 4GiB. */
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "number.h"
#include "options.h"
#include "cli.h"

static const char *program = "sdbackup";
static void usage(const char *o, const char *x, int l)
{
  fprintf(stderr, "Usage: %s [-q|--quiet] infile outfile\n", program);
  exit(1);
}

#define BLOCKSIZE 512

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

int main(int argc, char **argv)
{
  FILE *infile = 0, *outfile = 0;
  uint64_t i, n;
  uint8_t block[BLOCKSIZE];
  int errorcode;
  int percent = 0, old_percent = -1;
  int show_progress = 1;

  program = argv[0];
  parse_options(&argc, &argv, OPTIONS(
    FLAG_CALLBACK('h', "help", usage),
    FLAG('q', "quiet", show_progress, 0)
  ));

  if (argc < 3) {
    usage(0, 0, 0);
  }

  if (argv[1][0] == '-' && argv[1][1] == 0) {
    infile = stdin;
    show_progress = 0;
  }
  if (!infile) infile = fopen(argv[1], "rb");
  if (!infile) {
    fprintf(stderr, "Could not open file '%s': %s.\n", argv[1], strerror(errno));
    return -1;
  }

  /* Try reading the first block of the card. */
  if (!fread(block, BLOCKSIZE, 1, infile)) e_io(1);

  /* Check for constant fields in the first block. */
  if  (ld_u32_be(block)      != 0x74696d65ul /* time */
    || ld_u32_be(block + 29) != 0x61646472ul /* addr */
    || ld_u32_be(block + 37) != 0x73616d70ul /* samp */
    || ld_u32_be(block + 43) != 0x77726974ul /* writ */
    || ld_u32_be(block + 55) != 0x72656364ul /* recd */
    || ld_u32_be(block + 63) != 0x6c6f7374ul /* lost */
    || ld_u32_be(block + 71) != 0x74656d70ul /* temp */
    || ld_u32_be(block + 77) != 0x68756d69ul /* humi */
    || ld_u32_be(block + 83) != 0x6761696eul /* gain */
    || ld_u32_be(block + 91) != 0x636d6e74ul /* cmnt */
  ) e_fmt(2);

  /* Read the second block. */
  if (!fread(block, BLOCKSIZE, 1, infile)) e_io(3);

  /* Check for constant fields in the second block. */
  if  (ld_u32_be(block)      != 0x74696d65ul /* time */
    || ld_u32_be(block + 29) != 0x61646472ul /* addr */
    || ld_u32_be(block + 37) != 0x73616d70ul /* samp */
    || ld_u32_be(block + 43) != 0x77726974ul /* writ */
    || ld_u32_be(block + 55) != 0x72656364ul /* recd */
    || ld_u32_be(block + 63) != 0x6c6f7374ul /* lost */
    || ld_u32_be(block + 71) != 0x74656d70ul /* temp */
    || ld_u32_be(block + 77) != 0x68756d69ul /* humi */
    || ld_u32_be(block + 83) != 0x6761696eul /* gain */
    || ld_u32_be(block + 91) != 0x636d6e74ul /* cmnt */
  ) e_fmt(4);

  /* Read end address. */
  n = ld_u32_be(block + 33);

  /* Seek to the first block. */
  if (fseek(infile, 0, SEEK_SET) == -1) e_io(5);

  if (argv[2][0] == '-' && argv[2][1] == 0) {
    outfile = stdout;
    show_progress = 0;
  }
  if (!outfile) outfile = fopen(argv[2], "wb");
  if (!outfile) {
    fprintf(stderr, "Could not open file '%s': %s.\n", argv[2], strerror(errno));
    return -1;
  }

  /* Copy n blocks. */
  for (i = 0; i < n; ++i) {
    /* Read a block. */
    if (!fread(block, BLOCKSIZE, 1, infile)) e_io(6);
    /* Write a block. */
    if (!fwrite(block, BLOCKSIZE, 1, outfile)) e_io(7);
    /* Update progress. */
    if (show_progress && i % 4096 == 0) {
      percent = 100 * i / n;
      if (percent != old_percent) {
        progress(percent, 0);
        old_percent = percent;
      }
      fflush(0);
    }
  }
  if (show_progress) progress(100, 1);

  /* Close files. */
  if (infile) fclose(infile);
  if (outfile) fclose(outfile);

  return 0;

ioerror:
  if (infile) fclose(infile);
  if (outfile) fclose(outfile);
  fprintf(stderr, "IO error: There was an error while accessing the SD card."
    " (Error %d)\n", errorcode);
  return -1;

formaterror:
  if (infile) fclose(infile);
  if (outfile) fclose(outfile);
  fprintf(stderr, "Read error: The SD card content is malformed."
    " (Error %d)\n", errorcode);
  return -1;
}
