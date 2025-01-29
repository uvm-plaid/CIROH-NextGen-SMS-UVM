#include <stdint.h>
#include <TimeLib.h>

extern const int MAX_RESPONSE_LENGTH;
extern const int TIMEZONE_OFFSET;
extern const tmElements_t IRIDIUM_EPOCH;

namespace sat {
  int get_time(tmElements_t &time);
  int get_manufacturer();
}
