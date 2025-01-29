#include <stdint.h>;
#include <time.h>

extern const int MAX_RESPONSE_LENGTH;
extern const int TIMEZONE_OFFSET;
extern tm IRIDIUM_EPOCH;

namespace sat {
  int get_time(tm &time);
  int get_manufacturer();
  int send_receive(const char *command, char dst[], uint32_t sz);
}
