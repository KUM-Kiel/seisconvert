#ifndef WAV_H
#define WAV_H

#include <stdint.h>

/* Supported formats.
 * PCM is the only supported format at the moment. */
#define WAV_PCM 1

/* Structure for storing the WAV header. */
typedef struct wav_header_s wav_header_t;
struct wav_header_s {
  /* Sample format. Must be one of the supported formats above. */
  uint16_t format;
  /* Number of channels.
   * 1 = Mono
   * 2 = Stereo
   * More channels are possible (e.g. for surround sound). */
  uint16_t num_channels;
  /* Sample rate. Common choices for audio are 44100 or 48000. */
  uint32_t sample_rate;
  /* Sample bit depth. Must be 8, 12, 16, 24 or 32. */
  uint16_t bit_depth;
  /* Number of frames in the data stream. */
  uint32_t num_frames;
};

/* The required size for a WAV header.
 * Use this constant to allocate space for the buffer required by
 * wav_header_read and wav_header_write. */
#define WAV_HEADER_BYTES 44

/* Reads a wav_header_t from a WAV_HEADER_BYTES byte uint8_t buffer.
 * If everything went good, it returns the number of bytes read from buffer.
 * Returns -1 if the buffer is malformed. */
extern int wav_header_read(wav_header_t *header, const uint8_t *buffer);

/* Writes a wav_header_t to a WAV_HEADER_BYTES byte uint8_t buffer.
 * Returns the number of bytes written to buffer.
 * Returns -1 in case of an error. */
extern int wav_header_write(uint8_t *buffer, wav_header_t *header);

/* Lightweight datatype for reading and writing frames from a data stream.
 * Due to it's size it can be passed by value. */
typedef uint64_t wav_frame_config_t;

/* Returns a frame config that is used by the methods for reading and
 * writing signal frames.
 * header must be a initialised wav_header_t.
 * At least format, num_channels and bit_depth must be filled in. */
extern wav_frame_config_t wav_get_frame_config(wav_header_t *header);

/* Returns the frame size for the given frame config, i.e., the number
 * of bytes needed for each frame.
 * fc must be a wav_frame_config_t as returned form wav_get_frame_config(). */
extern int wav_get_frame_size(wav_frame_config_t fc);

/* Returns the number of channels for the given frame config.
 * fc must be a wav_frame_config_t as returned form wav_get_frame_config(). */
extern int wav_get_num_channels(wav_frame_config_t fc);

/* Reads a signal frame with the specified frame config from buffer[0], buffer[1], ...
 * buffer[wav_get_frame_size(fc) - 1] and stores it in samples[0], samples[1], ...
 * samples[wav_get_num_channels(fc) - 1].
 * It converts the samples to int32_t as appropiate for the frame config.
 * Returns the number of bytes read from buffer or, in case of an error, -1. */
extern int wav_read_int_frame(wav_frame_config_t fc, int32_t *samples, const uint8_t *buffer);

/* Writes a signal frame with the specified frame config to buffer[0], buffer[1], ...
 * buffer[wav_get_frame_size(fc) - 1]. The samples are taken from samples[0], samples[1], ...
 * samples[wav_get_num_channels(fc) - 1].
 * It converts the samples from int32_t to the appropiate format for the given frame config.
 * Returns the number of bytes read from buffer or, in case of an error, -1. */
extern int wav_write_int_frame(wav_frame_config_t fc, uint8_t *buffer, const int32_t *samples);

/* Reads a signal frame with the specified frame config from buffer[0], buffer[1], ...
 * buffer[wav_get_frame_size(fc) - 1] and stores it in samples[0], samples[1], ...
 * samples[wav_get_num_channels(fc) - 1].
 * It converts the samples to double as appropiate for the frame config.
 * Returns the number of bytes read from buffer or, in case of an error, -1. */
extern int wav_read_double_frame(wav_frame_config_t fc, double *samples, const uint8_t *buffer);

/* Writes a signal frame with the specified frame config to buffer[0], buffer[1], ...
 * buffer[wav_get_frame_size(fc) - 1]. The samples are taken from samples[0], samples[1], ...
 * samples[wav_get_num_channels(fc) - 1].
 * It converts the samples from double to the appropiate format for the given frame config.
 * Returns the number of bytes read from buffer or, in case of an error, -1. */
extern int wav_write_double_frame(wav_frame_config_t fc, uint8_t *buffer, const double *samples);

#endif
