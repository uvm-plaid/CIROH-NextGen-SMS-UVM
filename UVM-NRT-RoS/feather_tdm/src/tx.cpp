#include <SPI.h>
#include "shared.h"

static void tx_setup(RH_RF95& rf95) {
  Serial.println("Feather LoRa TX Test!");

  basic_setup(0, 1);
}

static int16_t packetnum = 0;

static void tx_loop(RH_RF95& rf95) {
  const int timeNode = 0;
  const int sendNode = 3;
  if (isCurrentNode(timeNode)) {
    if (!sendTime()) {
        Serial.println("Failed to send time!");
    }
    delay(nodeWindowMs);
  } else if (isCurrentNode(sendNode)) {
    Serial.println("Transmitting...");

    char radiopacket[20] = "Hello Girls#      ";
    itoa(packetnum++, radiopacket + 13, 10);
    Serial.print("Sending "); Serial.println(radiopacket);
    radiopacket[19] = 0;

    rf95.send((uint8_t *)radiopacket, 20);
    rf95.waitPacketSent();
    
    Serial.println("Packet sent!");
    delay(nodeWindowMs);
  } else {
    delay(nodeWindowMs / 2);
  }


}

vtable_t tx_vtable = {tx_setup, tx_loop};