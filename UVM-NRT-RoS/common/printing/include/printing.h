#ifndef PRINTING_H
#define PRINTING_H

namespace printing {

#define __DEBUG_PRINT true
#define __BUFFER_LENGTH 4096

/**
 * Debugging print.
 * @param message (const char *): C-style format string.
 * @param ... (va_list): Arguments for format specifiers in format string.
 */
void dbg(const char *format, ...);

/**
 * Debugging print statement that appends a newline character
 * at the end of the message.
 * @param message (const char *): C-style format string.
 * @param ... (va_list): Arguments for format specifiers in format string.
 */
void dbgln(const char *format, ...);
} // namespace printing

#endif /* PRINTING_H */
