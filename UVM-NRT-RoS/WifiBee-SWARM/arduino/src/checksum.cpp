/**
 * checksum.cpp
 * 
 * File containing function definitions for performing checksum calculations.
 * 
 * Created as part of the UVM CIROH project.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/16/24 
*/

#include "checksum.h"
#include <packets.h>

namespace checksum {
    
    uint8_t nmeaChecksum(char *data, size_t len) {
        size_t i = 0;
        uint8_t cs;

        if (data[0] == '$'){
            i++;
        }

        for (cs = 0; (i < len) && data [i]; i++){
            cs ^= data[i];
        }

        return cs;
    }

    uint16_t packetChecksum(packets::NodePacket &packet) {
        // Dummy return for now since we don't use this yet
        return 42;
    }
}
