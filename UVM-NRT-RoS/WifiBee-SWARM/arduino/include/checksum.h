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

#include <packets.h>

#include <cstring>
#include <stdint.h>

namespace checksum {
    /**
     * Function which calculates the checksum to append at the end of outgoing messages.
     * 
     * @param message (uint8_t []): Array of bytes to perform checksum on.
     * @param length (size_t): Length of the message being sent.
     * 
     * @returns (uint8_t): 8-bit checksum value to append to the end of the message.
    */
    uint8_t nmeaChecksum(uint8_t message[], size_t length);

    /**
     * Function which calculates the checksum for an outgoing packet. 
     * Used to verify packet integrity and operates over all fields prior
     * to the CRC.
     * 
     * @param packet (&packets::NodePacket): Packet to calculate the checksum for. 
     *  Reference to avoid copying.
     * 
     * @returns (uint16_t): 16-bit checksum value.
    */
   uint16_t packetChecksum(packets::NodePacket &packet);
}

#endif /* CHECKSUM_H */
