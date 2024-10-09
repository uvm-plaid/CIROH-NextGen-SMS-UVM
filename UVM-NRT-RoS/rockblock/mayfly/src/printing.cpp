/**
 * printing.cpp
 *
 * File containing implementations for debugging print functions.
 *
 * Authors: Jordan Bourdeau
 */

#include "printing.h"

#include <SPI.h>
#include <stdarg.h>

namespace printing {

// Max buffer length
static char PRINT_BUFFER[BUFFER_LENGTH];

#define TRY_BUFFER_WRITE(buffer, buffer_length, format, args)                  \
  {                                                                            \
    size_t end = vsnprintf(buffer, buffer_length - 1, format, args);           \
    if (end > BUFFER_LENGTH) {                                                 \
      Serial.print("String too larger for buffer size.");                      \
      return;                                                                  \
    } else {                                                                   \
      buffer[end] = '\0';                                                      \
    }                                                                          \
  }

// Print using a pretty large fixed size buffer
// No handling for anything beyond buffer size currently other than
// throwing up our hands and not printing.
void dbg(const char *format, ...) {
  if (DEBUG_PRINT) {
    va_list args;
    va_start(args, format);

    TRY_BUFFER_WRITE(PRINT_BUFFER, BUFFER_LENGTH, format, args);
    Serial.print(PRINT_BUFFER);

    va_end(args);
  }
}

void dbgln(const char *format, ...) {
  if (DEBUG_PRINT) {
    va_list args;
    va_start(args, format);

    TRY_BUFFER_WRITE(PRINT_BUFFER, BUFFER_LENGTH, format, args);
    Serial.println(PRINT_BUFFER);

    va_end(args);
  }
}

} // namespace printing
