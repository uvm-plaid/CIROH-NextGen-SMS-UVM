#include "platform/feather/rf95.h"
#include "tdm.h"

#ifdef ARDUINO_SAMD_MKRWAN1310
RH_RF95 rf95(10, 9, 2);
#else 
RH_RF95 rf95(RFM95_CS, RFM95_INT);
#endif

void basic_setup(uint8_t thisAddress, uint8_t toAddress) {
    #ifdef ARDUINO_SAMD_MKRWAN1310
    #else
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);

  Serial.println("Feather LoRa RX Test!");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
  rf95.setThisAddress(thisAddress);
  rf95.setHeaderFrom(thisAddress);
  rf95.setHeaderTo(toAddress);
  #endif
}

void blink() {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
}
