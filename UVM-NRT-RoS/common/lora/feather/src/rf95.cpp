#include "rf95.h"
#include "tdm.h"

#ifdef ARDUINO_SAMD_MKRWAN1310
RH_RF95 rf95(10, 9, 2);
#else 
RH_RF95 rf95(RFM95_CS, RFM95_INT);
#endif

void blink() {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
}
