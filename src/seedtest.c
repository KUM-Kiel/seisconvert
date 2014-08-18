#include <stdio.h>
#include <stdlib.h>
#include "seed.h"

void alloc_cb(seedfile_t *f, uint64_t size, seed_buffer_t *buf)
{
  buf->data = malloc(size);
  buf->size = size;
}

void data_cb(seedfile_t *f, int64_t size, seed_buffer_t *buf)
{
  data_record_header h;
  blockette_1000 _1000;
  read_data_record_header(&h, buf->data);
  read_blockette_1000(&_1000, buf->data + h.blockette_offset);
  free(buf->data);
}

int main(int argc, char **argv)
{
  if (argc < 2) return -1;
  seedfile_t *f = seed_open(argv[1]);
  if (!f) {
    fprintf(stderr, "Invalid file: %s.\n", argv[1]);
    return -1;
  }
  seed_begin_read(f, data_cb, alloc_cb);
  seed_close(f);
  return 0;
}
