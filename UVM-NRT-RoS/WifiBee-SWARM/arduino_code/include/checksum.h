/**
 * checksum.h
 * 
 * Header file containing functions for performing checksum calculations.
 * 
 * Created as part of the UVM CIROH project.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/16/24 
*/

#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <cstring>
#include <stdint.h>

namespace checksum {
    /**
     * Function which calculates the checksum to append at the end of outgoing messages.
     * 
     * @param message (const char *): Zero-terminated message string.
     * @param length (size_t): Length of the message being sent.
     * 
     * @returns (uint8_t): 8-bit checksum value to append to the end of the message.
    */
    uint8_t nmeaChecksum(const char *message, size_t length);
}

#endif /* CHECKSUM_H */
