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

namespace checksum {
    uint8_t nmeaChecksum(const char *sz, size_t len) {
        size_t i = 0;
        uint8_t cs;

        if (sz [0] == '$'){
            i++;
        }

        for (cs = 0; (i < len) && sz [i]; i++){
            cs ^= ((uint8_t) sz [i]);
        }

        return cs;
    }
}
