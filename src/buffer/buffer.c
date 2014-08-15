#include "buffer.h"

#include <stdlib.h>

typedef uint8_t u8;
typedef uint64_t u64;

buffer_t *buffer_new(u64 size)
{
  /* Allocate space for the buffer. sizeof(buffer_t) already includes one byte
   * of the internal ringbuffer, so we need another size - 1 bytes. */
  buffer_t *buffer = malloc(sizeof(buffer_t) + size - 1);
  /* Check for success. */
  if (!buffer) return 0;
  /* Initialise fields. */
  buffer->size = size;
  buffer->pos = 0;
  buffer->data = 0;
  buffer->streampos = 0;
  /* Return the buffer. */
  return buffer;
}

void buffer_delete(buffer_t *buffer)
{
  /* free(NULL) is valid. */
  free(buffer);
}

u64 buffer_position(buffer_t *buffer)
{
  /* Returns 0 if the buffer is invalid. */
  return buffer ? buffer->streampos : 0;
}

u64 buffer_data(buffer_t *buffer)
{
  /* Returns 0 if the buffer is invalid. */
  return buffer ? buffer->data : 0;
}

u64 buffer_space(buffer_t *buffer)
{
  /* Returns 0 if the buffer is invalid. */
  return buffer ? buffer->size - buffer->data : 0;
}

u64 buffer_read(buffer_t *buffer, u8 *x, u64 xlen)
{
  u64 i = 0;
  /* Error checking. */
  if (!buffer || !x) return 0;
  /* Read up to xlen or data bytes, whichever is less. */
  while (i < xlen && i < buffer->data) {
    /* Position wraps over at size. */
    x[i] = buffer->buffer[(buffer->pos + i) % buffer->size];
    ++i;
  }
  /* Return the number of bytes read. */
  return i;
}

u64 buffer_write(buffer_t *buffer, const u8 *x, u64 xlen)
{
  u64 i = 0;
  /* Error checking. */
  if (!buffer || !x) return 0;
  /* Write up to xlen or size - data bytes, whichever is less. */
  while (i < xlen && i < buffer->size - buffer->data) {
    buffer->buffer[(buffer->pos + buffer->data + i) % buffer->size] = x[i];
    ++i;
  }
  /* Advance write position. */
  buffer->data += i;
  /* Return the number of bytes written. */
  return i;
}

u64 buffer_advance(buffer_t *buffer, u64 n)
{
  /* Error checking. */
  if (!buffer) return 0;
  /* Only advance up to data bytes. */
  if (n > buffer->data)
    n = buffer->data;
  /* Advance positions. */
  buffer->pos += n;
  buffer->streampos += n;
  buffer->data -= n;
  /* Return the number of bytes the buffer was advanced. */
  return n;
}
