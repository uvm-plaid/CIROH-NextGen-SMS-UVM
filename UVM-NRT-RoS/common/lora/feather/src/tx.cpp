#include <SPI.h>
#include "rf95.h"
#include "tdm.h"

static void tx_setup(RHGenericDriver& rf95) {
  Serial.println("Feather LoRa TX Test!");

  basic_setup(0, 1);
}

static int16_t packetnum = 0;

static void tx_loop(RHGenericDriver& rf95) {
  const int timeNode = 0;
  const int sendNode = 3;
  Serial.println("tx loop");

  if (isCurrentNode(timeNode)) {
    /*
    Serial.println("Time to send!!");
    if (!sendTime()) {
        Serial.println("Failed to send time!");
    }
    */
    delay(nodeWindowMs);
  } else if (isCurrentNode(sendNode)) {
    
    Serial.println("Transmitting...");

    const char* radiopacket = Sprintf("hello %d", packetnum++);
    Serial.print("Sending "); Serial.println(radiopacket);

    rf95.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf95.waitPacketSent();
    
    Serial.println("Packet sent!");
    delay(nodeWindowMs);
    
  } else {
    delay(nodeWindowMs / 2);
  }
}

vtable_t tx_vtable = {tx_setup, tx_loop};
