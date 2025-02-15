#include "printing.h"
#include "satellite.h"

#include <Arduino.h>
#include <TimeLib.h>

// Mobile terminated buffer (receiving responses)
// Can technically be as large as 1960 bytes, but will be very unlikely to
// exceed 135 or 270
const uint32_t MAX_RESPONSE_LEN = 270;
static char MT_BUF[MAX_RESPONSE_LEN];

// Mobile originated buffer (sending packets)
const uint32_t MAX_CMD_LEN = 120;
const uint32_t MAX_PACKET_LEN = 340;

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

static sat::SatCode send(const char *command) {
    ECHO("Sending \"%s\"\n", command);
    uint32_t cmd_len = strlen(command);
    if (cmd_len > MAX_CMD_LEN) {
        return sat::SatCode::CmdTooLong;
    }

    uint32_t n_written = Serial1.write(command, cmd_len);

    // Write a carriage return to signal end of command and wait for response.
    Serial1.write("\r");
    ++n_written;

    ECHO("Wrote %d bytes\n", n_written);
    return sat::SatCode::Okay;
}

static sat::SatCode receive(char dst[], uint32_t sz, bool block = true) {
    size_t index = 0;
    // Imperfect blocking method which waits half a second after serial becomes
    // available to make sure all the bytes have been transmitted.
    if (block) {
        while (!Serial1.available());
        delay(500);
    }

    while (Serial1.available() && index < sz) {
        dst[index++] = Serial1.read();
    }
    if (index == sz) {
        return sat::SatCode::ResponseTooLong;
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

/**
 * Internal function without a public API.
 * Send a command, wait for a response, then write it to the destination
 * buffer and print it to the screen.
 *
 * @param command (const char *): Command string.
 * @param dst (char []): Destination buffer to receive response into.
 * @param sz (uint32_t): Size of the destination buffer.
 * @param block (bool): Block until all data is available.
 *
 * @returns (SatCode): Return code.
*/
static sat::SatCode send_receive(
    const char *command, 
    char dst[], 
    uint32_t sz,
    bool block = true
) {
    sat::SatCode rc;
    if ((rc = send(command)) != sat::SatCode::Okay) {
        return rc;
    }
    return receive(dst, sz, block);
}

sat::SatCode sat::initiate_transfer(sat::SbdixResponse &response) {
    sat::SatCode rc;
    // This has to be a blocking call because it takes a while
    if ((rc = send_receive("AT+SBDIX", MT_BUF, MAX_RESPONSE_LEN, true)) 
        != sat::SatCode::Okay) {
      return rc;
    }

    // Parse the response
    static char sub[] = "+SBDIX:";
    char *start = strstr(MT_BUF, sub) + sizeof(sub);
    if (!start) {
        ECHO("Did not find time. Instead got response:\n%s", MT_BUF);
        return sat::SatCode::InvalidResponse;
    }
    // Keep splitting the response tuple
    static uint32_t base = 10;
    static char delim[] = ",";
    char *ptr = strtok(start, delim);
    ECHO("MO Status: %s", ptr);
    auto mo_status = static_cast<sat::SbdixResponse::MoStatus>(strtoul(ptr, nullptr, base));
    if (mo_status == sat::SbdixResponse::MoStatus::NoNetworkService) {
        return sat::SatCode::NetworkErr;
    }
    if ((ptr = strtok(nullptr, delim)) == nullptr) { return sat::SatCode::InvalidResponse; }
    ECHO("MT Status: %s", ptr);
    auto mt_status = static_cast<sat::SbdixResponse::MtStatus>(strtoul(ptr, nullptr, base));
    if ((ptr = strtok(nullptr, delim)) == nullptr) { return sat::SatCode::InvalidResponse; }
    ECHO("MOMSN: %s", ptr);
    uint16_t momsn = strtoul(ptr, nullptr, base);
    if ((ptr = strtok(nullptr, delim)) == nullptr) { return sat::SatCode::InvalidResponse; }
    ECHO("MT Length: %s", ptr);
    uint32_t mt_length = strtoul(ptr, nullptr, base);
    if ((ptr = strtok(nullptr, delim)) == nullptr) { return sat::SatCode::InvalidResponse; }
    ECHO("MT Queued: %s", ptr);
    uint32_t mt_queued = strtoul(ptr, nullptr, base);
    if ((ptr = strtok(nullptr, delim)) != nullptr) { return sat::SatCode::InvalidResponse; }
    ECHO("Extra: %s", ptr);
   
    // Set the fields
    response.mo_status = mo_status;
    response.mt_status = mt_status;
    response.momsn = momsn;
    response.mt_length = mt_length;
    response.mt_queued = mt_queued;

    return sat::SatCode::Okay;
}

sat::SatCode sat::send_packet(const uint8_t buf[], uint16_t nbytes) {
    sat::SatCode rc;
    uint32_t checksum = 0;
    static char cmd[] = "AT+SBDWB=";
    // Command, up to 3 digits for len, and 1 for CR
    static char binary_write[sizeof(cmd) + 4];
    static char ready[] = "READY\r\n"; 
    sat::SbdixResponse response;

    // Need room for the '\r' and 2-byte checksum
    if (nbytes > MAX_PACKET_LEN - 3) {
        return sat::SatCode::CmdTooLong;
    }

    // Format binary write command
    snprintf(binary_write, sizeof(binary_write), "%s%d\r", cmd, nbytes);
    if ((rc = send_receive(binary_write, MT_BUF, MAX_RESPONSE_LEN, true)) 
        != sat::SatCode::Okay) {
        ECHO("Binary write error!");
        return rc;
    }

    // Make sure response contains the ready string
    // Safety for not using strnstr (not part of stdlib): Bounds checked
    if (strstr(MT_BUF, ready) == nullptr) {
        ECHO("Did not find ready string in response!");
        return sat::SatCode::InvalidResponse;
    }

    // Send the data, keeping track of the checksum
    for (uint32_t i = 0; i < nbytes; ++i) {
        checksum += buf[i];
        Serial1.write(buf[i]);
    }
    checksum &= 0xFFFF;  // Truncate to lower 16 bits
    Serial1.write(static_cast<uint16_t>(checksum));

    if ((rc = receive(MT_BUF, MAX_RESPONSE_LEN, true)) 
        != sat::SatCode::Okay) {
        ECHO("Error receiving response after write!");
        return rc;
    }
    
    // Keep initiating transfers if there is a network error
    // TODO: Add some limit? Do we want to use this response for anything?
    while ((rc = sat::initiate_transfer(response)) != sat::SatCode::Okay) {
        if (rc == sat::SatCode::NetworkErr) {
            delay(1000);
        } else {
            return rc;
        }
    }
    return sat::SatCode::Okay;
}

sat::SatCode sat::get_manufacturer() {
    return send_receive("AT+CGMI", MT_BUF, MAX_RESPONSE_LEN, true);
}

sat::SatCode sat::get_time(tmElements_t &time) {
    sat::SatCode rc;
    if ((rc = send_receive("AT-MSSTM", MT_BUF, MAX_RESPONSE_LEN, true)) != sat::SatCode::Okay) {
      return rc;
    }
    // Expect a response in the form of 0xXXXXXXXX which is a 32-bit hex
    // ascii character string representing the number of 90 millisecond
    // intervals that have elapsed since the Iridium epoch.
    static char sub[] = "MSSTM: ";
    char *start = strstr(MT_BUF, sub) + sizeof(sub) - 1;
    if (!start) {
        ECHO("Did not find time. Instead got response:\n%s", MT_BUF);
        return sat::SatCode::InvalidResponse;
    }

    uint64_t intervals = strtoul(start, nullptr, 16);
    if (intervals == 0) {
        ECHO("Could not parse response. No network service.", MT_BUF);
        return sat::SatCode::NetworkErr;
    }

    // Calculate total seconds elapsed since the Iridium epoch
    uint64_t total_ms = intervals * 90; // 90 milliseconds per interval
    uint64_t total_seconds = total_ms / 1000;
    ECHO("Seconds: %ul", total_seconds);

    // Calculate current time
    time_t epoch_time = makeTime(IRIDIUM_EPOCH);
    epoch_time += total_seconds;
    epoch_time += TIMEZONE_OFFSET;
    breakTime(epoch_time, time);

    return sat::SatCode::Okay;
}
