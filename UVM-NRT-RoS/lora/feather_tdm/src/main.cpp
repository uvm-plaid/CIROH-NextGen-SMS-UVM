#include <SPI.h>
#include <RH_RF95.h>
#include <LoRa.h>

#include "shared.h"

const double FREQ = 915E6;

// Change this const to change board deploy to Tx or Rx.
const vtable_t tab = tx_vtable;

int counter = 0;

void sendSetup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void sendLoop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}

void recvSetup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void recvLoop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void setup() {
  // Serial.println("Setting up.");
  // tab.setup(rf95);
  recvSetup();
}

void loop() {
  // Serial.println("Looping.");
  // tab.loop(rf95);
  recvLoop();
}