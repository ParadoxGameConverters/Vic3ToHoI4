#include "stddef.h"

/**
 * A MeltedBuffer holds the result of a melting operation (binary to plaintext translation).
 * Either the melting operation succeeded, and the buffer is filled with plaintext or it contains
 * an error.
 */
typedef struct MeltedBuffer MeltedBuffer;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * A non-zero return value indicates an error with the melted buffer
 *
 * A non-zero status code can occur from the following:
 *
 *  - An early EOF
 *  - Invalid format encountered
 *  - Too many close delimiters
 *
 * # Safety
 *
 * Must pass in a valid pointer to a `MeltedBuffer`
 */
int rakaly_melt_error_code(const struct MeltedBuffer *res);

/**
 * Destroys a `MeltedBuffer` once you are done with it.
 *
 * # Safety
 *
 * Must pass in a valid pointer to a `MeltedBuffer`
 */
void rakaly_free_melt(struct MeltedBuffer *res);

/**
 * Returns the length of the melted data in bytes.
 *
 * # Safety
 *
 * Must pass in a valid pointer to a `MeltedBuffer`
 */
size_t rakaly_melt_data_length(const struct MeltedBuffer *res);

/**
 * Writes the melted data into a provided buffer that is a given length.
 *
 * Returns the number of bytes copied from the melted data to the provided buffer.
 *
 * If the buffer is not long enough for the melted data, then 0 is returned.
 *
 * If the melted data or provided buffer are null, then 0 is returned.
 *
 * # Safety
 *
 * - Must pass in a valid pointer to a `MeltedBuffer`
 * - Given buffer must be at least the given length in size
 */
size_t rakaly_melt_write_data(const struct MeltedBuffer *res, char *buffer, size_t length);

/**
 * Melts binary encoded ironman data into normal plaintext data that can be understood by EU4
 * natively. The melted buffer, when written out will contain windows-1252 encoded plaintext.
 *
 * Parameters:
 *
 *  - data: Pointer to immutable data that represents the ironman data. The data can be a ironman
 *  savefile in a zip format, in which case rakaly will take care of unzipping, melting, and
 *  concatenating the data into a single plaintext output. The pointer can point to ironman data
 *  that has already been unzipped.
 *  - data_len: Length of the data indicated by the data pointer. It is undefined behavior if the
 *  given length does not match the actual length of the data
 *
 * If an unknown token is encountered and rakaly doesn't know how to convert it to plaintext there
 * are two possible outcomes:
 *
 *  - If the token is part of an object's key then key and value will not appear in the plaintext
 *  output
 *  - Else the object value (or array value) will be string of "__unknown_x0$z" where z is the
 *  hexadecimal representation of the unknown token.
 */
struct MeltedBuffer *rakaly_eu4_melt(const char *data_ptr, size_t data_len);

/**
 * Melts binary encoded CK3 data into normal plaintext data. The melted buffer will contain utf-8 encoded
 * text.
 *
 * Parameters:
 *
 *  - data: Pointer to immutable data that represents the binary data. The data can be:
 *    - autosave save
 *    - ironman save
 *    - binary data
 *  - data_len: Length of the data indicated by the data pointer. It is undefined behavior if the
 *  given length does not match the actual length of the data
 *
 * If an unknown token is encountered and rakaly doesn't know how to convert it to plaintext there
 * are two possible outcomes:
 *
 *  - If the token is part of an object's key then key and value will not appear in the plaintext
 *  output
 *  - Else the object value (or array value) will be string of "__unknown_x0$z" where z is the
 *  hexadecimal representation of the unknown token.
 */
struct MeltedBuffer *rakaly_ck3_melt(const char *data_ptr,
                                     size_t data_len);

/**
 * Melts binary encoded Imperator data into normal plaintext data. The melted buffer will contain utf-8 encoded
 * text.
 *
 * Parameters:
 *
 *  - data: Pointer to immutable data that represents the binary data. The data can be:
 *    - a save file
 *    - binary data from already extracted gamestate
 *  - data_len: Length of the data indicated by the data pointer. It is undefined behavior if the
 *  given length does not match the actual length of the data
 *
 * If an unknown token is encountered and rakaly doesn't know how to convert it to plaintext there
 * are two possible outcomes:
 *
 *  - If the token is part of an object's key then key and value will not appear in the plaintext
 *  output
 *  - Else the object value (or array value) will be string of "__unknown_x0$z" where z is the
 *  hexadecimal representation of the unknown token.
 */
struct MeltedBuffer *rakaly_imperator_melt(const char *data_ptr,
                                           size_t data_len);

/**
 * Melts binary encoded HOI4 data into normal plaintext data. The melted buffer will contain utf-8 encoded
 * text.
 *
 * Parameters:
 *
 *  - data: Pointer to immutable data that represents the binary data
 *  - data_len: Length of the data indicated by the data pointer. It is undefined behavior if the
 *  given length does not match the actual length of the data
 *
 * If an unknown token is encountered and rakaly doesn't know how to convert it to plaintext there
 * are two possible outcomes:
 *
 *  - If the token is part of an object's key then key and value will not appear in the plaintext
 *  output
 *  - Else the object value (or array value) will be string of "__unknown_x0$z" where z is the
 *  hexadecimal representation of the unknown token.
 */
struct MeltedBuffer *rakaly_hoi4_melt(const char *data_ptr,
                                      size_t data_len);

/**
 * Melts binary encoded vic3 data into normal plaintext data. The melted buffer will contain utf-8 encoded
 * text.
 *
 * Parameters:
 *
 *  - data: Pointer to immutable data that represents the binary data
 *  - data_len: Length of the data indicated by the data pointer. It is undefined behavior if the
 *  given length does not match the actual length of the data
 *
 * If an unknown token is encountered and rakaly doesn't know how to convert it to plaintext there
 * are two possible outcomes:
 *
 *  - If the token is part of an object's key then key and value will not appear in the plaintext
 *  output
 *  - Else the object value (or array value) will be string of "__unknown_x0$z" where z is the
 *  hexadecimal representation of the unknown token.
 */
struct MeltedBuffer *rakaly_vic3_melt(const char *data_ptr,
                                      size_t data_len);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#ifdef __cplusplus
#ifndef RAKALY_WRAPPER_H
#define RAKALY_WRAPPER_H

#include <stdexcept>
#include <string>

namespace rakaly {

inline std::string meltFinish(MeltedBuffer *melt) {
  int result = rakaly_melt_error_code(melt);
  if (result) {
    rakaly_free_melt(melt);
    throw std::runtime_error("librakaly returned an error: " +
                             std::to_string(result));
  }

  size_t len = rakaly_melt_data_length(melt);
  std::string str(len, ' ');

  if (rakaly_melt_write_data(melt, str.data(), len) != len) {
    rakaly_free_melt(melt);
    throw std::runtime_error("librakaly failed to copy data.");
  }

  rakaly_free_melt(melt);
  return str;
}

inline std::string meltEU4(const std::string &encrypted) {
  return meltFinish(rakaly_eu4_melt(encrypted.c_str(), encrypted.length()));
}

inline std::string meltCK3(const std::string &encrypted) {
  return meltFinish(rakaly_ck3_melt(encrypted.c_str(), encrypted.length()));
}

inline std::string meltImperator(const std::string &encrypted) {
  return meltFinish(rakaly_imperator_melt(encrypted.c_str(), encrypted.length()));
}

inline std::string meltHoi4(const std::string &encrypted) {
  return meltFinish(rakaly_hoi4_melt(encrypted.c_str(), encrypted.length()));
}

inline std::string meltVic3(const std::string &encrypted) {
  return meltFinish(rakaly_vic3_melt(encrypted.c_str(), encrypted.length()));
}

} // namespace rakaly

#endif
#endif // __cplusplus
