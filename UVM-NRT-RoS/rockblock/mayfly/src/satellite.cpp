#include "loraDevice.h"
#include "printing.h"
#include "satellite.h"

#include <Arduino.h>
#include <TimeLib.h>

// Mobile terminated buffer (receiving responses)
// Can technically be as large as 1960 bytes, but will be very unlikely to
// exceed 135 or 270
const uint32_t MAX_RESPONSE_LENGTH = 270;
static char MT_BUF[MAX_RESPONSE_LENGTH];

// Mobile originated buffer (sending packets)
const uint32_t MAX_PACKET_LENGTH = 340;
static char MO_BUF[MAX_PACKET_LENGTH];

// Hardcoded timezone EST offset and Iridium epoch
const int TIMEZONE_OFFSET = -60 * 60 * 5;
const tmElements_t IRIDIUM_EPOCH = {
    55,  // Second
    23,  // Minute
    14,  // Hour
    1,   // Weekday (Sunday)
    11,  // Day
    5,   // Month
    2014 - 1970,  // Year - offset from 1970
};

// Debug printing
bool __DEBUG_ECHO = true;
void sat::set_echo(bool echo) { __DEBUG_ECHO = echo; }
#define ECHO(format, ...) do { if (__DEBUG_ECHO) printing::dbgln(format, ##__VA_ARGS__); } while (0)

/**
 * Internal function without a public API.
 * Send a command, wait for a response, then write it to the destination
 * buffer and print it to the screen.
 *
 * @param command (const char *): Command string.
 * @param dst (char []): Destination buffer to receive response into.
 * @param sz (uint32_t): Size of the destination buffer.
 *
 * @returns (SatCode): Return code.
*/
static sat::SatCode send_receive(const char *command, char dst[], uint32_t sz) {
    ECHO("Sending \"%s\"\n", command);
    uint32_t cmd_len = strlen(command);
    if (cmd_len > sz) {
        return sat::SatCode::CmdTooLong;
    }

    uint32_t n_written = Serial1.write(command, cmd_len);

    // Write a carriage return to signal end of command and wait for response.
    Serial1.write("\r");
    ++n_written;
    delay(1000);

    ECHO("Wrote %d bytes\n", n_written);

    size_t index = 0;
    while (Serial1.available()) {
        dst[index++] = Serial1.read();
    }
    if (index > 0) {
        dst[index] = '\0'; // Null terminate the string
        ECHO("Received %d bytes: %s\n", index, dst);
    } else {
        ECHO("Error reading from the serial port.\n");
        return sat::SatCode::RecvErr;
    }
    return sat::SatCode::Okay;
}

sat::SbdixResponse sat::initiate_transfer() {
    sat::SbdixResponse response;
    return response;
}

sat::SatCode sat::send_packet(LoraPacket packet) {
    return sat::SatCode::Okay;
}

sat::SatCode sat::get_manufacturer() {
    return send_receive("AT+CGMI", MT_BUF, MAX_RESPONSE_LENGTH);
}

sat::SatCode sat::get_time(tmElements_t &time) {
    sat::SatCode rc;
    if ((rc = send_receive("AT-MSSTM", MT_BUF, MAX_RESPONSE_LENGTH)) != sat::SatCode::Okay) {
      return rc;
    }
    // Expect a response in the form of 0xXXXXXXXX which is a 32-bit hex
    // ascii character string representing the number of 90 millisecond
    // intervals that have elapsed since the Iridium epoch.
    static char sub[] = "MSSTM: ";
    char *start = strstr(MT_BUF, sub) + strlen(sub);
    if (!start) {
        ECHO("Did not find time. Instead got response:\n%s", MT_BUF);
        return sat::SatCode::InvalidResponse;
    }

    uint64_t intervals = strtoul(start, nullptr, 16);

    // Calculate total seconds elapsed since the Iridium epoch
    uint64_t total_ms = intervals * 90; // 90 milliseconds per interval
    uint64_t total_seconds = total_ms / 1000;

    // Calculate current time
    time_t epoch_time = makeTime(IRIDIUM_EPOCH);
    epoch_time += total_seconds;
    epoch_time += TIMEZONE_OFFSET;
    breakTime(epoch_time, time);

    return sat::SatCode::Okay;
}
