#include <SPI.h>
#include "shared.h"

static void rx_setup(RH_RF95& rf95) {
  basic_setup(1, 0);
  Serial.println("Feather LoRa RX Test!");
}

static void rx_loop(RH_RF95& rf95) {
  if (rf95.available()) {
    static uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    static uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      Serial.print("Received: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      Serial.print("FROM: ");
      Serial.print(rf95.headerFrom(), DEC);
      Serial.print(", TO: ");
      Serial.println(rf95.headerTo(), DEC);
      blink();
    } 
    else {
      Serial.println("Receive failed");
    }

    
  }
}

vtable_t rx_vtable = {rx_setup, rx_loop};