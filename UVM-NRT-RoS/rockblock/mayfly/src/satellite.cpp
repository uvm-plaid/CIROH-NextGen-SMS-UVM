#include <printing.h>
#include <satellite.h>

#include <Arduino.h>

const int MAX_RESPONSE_LENGTH = 256;
static char RESPONSE_BUFFER[MAX_RESPONSE_LENGTH];
const int TIMEZONE_OFFSET = -60 * 60 * 4;

static tm IRIDIUM_EPOCH = {
    55,  // tm_sec
    23,  // tm_min
    14,  // tm_hour
    11,  // tm_mday
    4,   // tm_mon (0 = January, so 4 = May)
    2014 - 1900,  // tm_year (years since 1900)
    0,   // tm_wday (not set)
    0,   // tm_yday (not set)
    -1   // tm_isdst (let the library determine DST)
};

  /**
   * Send a command, wait for a response, then write it to the destination
   * buffer and print it to the screen.
   *
   * @param command (const char *): Command string.
   * @param dst (char []): Destination buffer to receive response into.
   * @param sz (uint32_t): Size of the destination buffer.
   *
   * @returns (int): Return code.
   */
    static int sat::send_receive(const char *command, char dst[], uint32_t sz) {
        printing::dbgln("Sending \"%s\"\n", command);
        uint32_t cmd_len = strlen(command);
        if (cmd_len > sz) {
            return 1;
        }

        uint32_t n_written = Serial1.write(command, cmd_len);

        // Write a carriage return to signal end of command and wait for response.
        Serial1.write("\r");
        ++n_written;
        delay(1000);

        printing::dbgln("Wrote %d bytes\n", n_written);

        size_t index = 0;
        while (Serial1.available()) {
            dst[index++] = Serial1.read();
        }
        if (index > 0) {
            dst[index] = '\0'; // Null terminate the string
            printing::dbgln("Received %d bytes: %s\n", index, dst);
        } else {
            printing::dbgln("Error reading from the serial port.\n");
            return 2;
        }
        return 0;
    }

    int sat::get_manufacturer() {
        if (int rc = send_receive("AT+CGMI", RESPONSE_BUFFER, MAX_RESPONSE_LENGTH)) {
            return rc;
        }
        printing::dbgln("Manufacturer:\n%s", RESPONSE_BUFFER);
        return 0;
    }

    int sat::get_time(tm &time) {
        static char get_time[] = "AT-MSSTM";
        static char sub[] = "MSSTM: ";

        if (int rc = send_receive(get_time, RESPONSE_BUFFER, MAX_RESPONSE_LENGTH)) {
          return rc;
        }
        // Expect a response in the form of 0xXXXXXXXX which is a 32-bit hex
        // ascii character string representing the number of 90 millisecond
        // intervals that have elapsed since the Iridium epoch.
        char *start = strstr(RESPONSE_BUFFER, sub) + strlen(sub);
        if (!start) {
            printing::dbgln("Did not find time. Instead got response:\n%s",
                  RESPONSE_BUFFER);
            return 1;
        }

        uint64_t intervals = strtoul(start, nullptr, 16);

        // Calculate total seconds elapsed since the Iridium epoch
        uint64_t total_ms = intervals * 90; // 90 milliseconds per interval
        uint64_t total_seconds = total_ms / 1000;

        // TODO: Read RTC time here
        // Convert to time_t for easier manipulation
        /*time_t epoch_time = std::mktime(&IRIDIUM_EPOCH);*/
        /*epoch_time += total_seconds;*/
        /*epoch_time += TIMEZONE_OFFSET;*/
        /**/
        /*// Convert to local time (EST)*/
        /*tm *local_time = std::localtime(&epoch_time);*/
        /*time = *local_time;*/

        return 0;
    }
