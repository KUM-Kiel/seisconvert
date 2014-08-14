#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

/* A buffer that can store up to size bytes of binary data. */
typedef struct buffer_s buffer_t;
struct buffer_s {
  uint64_t size;
  uint64_t pos;
  uint64_t data;
  uint64_t streampos;
  uint8_t buffer[1];
};

/* Creates a new buffer with the given size.
 * Returns NULL if there is insufficient memory available. */
extern buffer_t *buffer_new(uint64_t size);

/* Deletes the given buffer and frees it's ressources.
 * If the buffer has already been deleted the behaviour is undefined. */
extern void buffer_delete(buffer_t *buffer);

/* Returns the current position of the buffer.  This is the position on which
 * reading operations are started. */
extern uint64_t buffer_position(buffer_t *buffer);

/* Returns the amount of data available in the buffer. */
extern uint64_t buffer_data(buffer_t *buffer);

/* Reads up to xlen bytes from the buffer starting at it's current position and
 * stores them in x[0], x[1], ..., x[xlen - 1].  If there are less than xlen
 * bytes in the buffer, they are all written to x.  In any case it returns the
 * number of bytes actually written to x. */
extern uint64_t buffer_read(buffer_t *buffer, uint8_t *x, uint64_t xlen);

/* Reads up to xlen bytes from x[0], x[1], ..., x[xlen - 1] and writes them to
 * the buffer.  If there is not enough free space in the buffer, the maximum
 * possible number of bytes is stored.  Returns the number of bytes actually
 * written to the buffer. */
extern uint64_t buffer_write(buffer_t *buffer, const uint8_t *x, uint64_t xlen);

/* Advances the buffers current position by n bytes.  Bytes before the new
 * position can not be read any more after this operation.  It is not possible
 * to advance the buffer further than there is data available.  If a larger n
 * is supplied, the buffer will only be advanced as far as possible.
 * In any case returns the number of bytes the buffer was actually advanced. */
extern uint64_t buffer_advance(buffer_t *buffer, uint64_t n);

#endif
