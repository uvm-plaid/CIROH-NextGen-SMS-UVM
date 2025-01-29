#include "loraDevice.h"

#include <stdint.h>
#include <TimeLib.h>

extern const int MAX_RESPONSE_LENGTH;
extern const int TIMEZONE_OFFSET;
extern const tmElements_t IRIDIUM_EPOCH;

namespace sat {
    enum class SatCode {
        Okay,
        CmdTooLong,
        RecvErr,
        InvalidResponse,
    };

    void set_echo(bool echo); 
    sat::SatCode send_packet(LoraPacket packet);
    sat::SatCode get_time(tmElements_t &time);
    sat::SatCode get_manufacturer();
}
