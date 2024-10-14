#include "loraDevice.h"
#include <Arduino.h>

// LilyGo stuff -- to be removed
#include <U8g2lib.h>
#include "platform/lilygo/LoRaBoards.h"


const double FREQ = 915E6;

/*
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
*/


LoraDevice* lora = nullptr;

const bool isReceiving = false;

void setup() {
  // Serial.println("Setting up.");
  // tab.setup(rf95);
  // recvSetup();
  lora = LoraDevice::getInstance();
  lora->setup();
}

void doReceiveLoop();
void doSendLoop();

void loop() {
  // Serial.println("Looping.");
  // tab.loop(rf95);
  // recvLoop();
  if (isReceiving) {
    doReceiveLoop();
  } else {
    doSendLoop();
  }
}

char* Sprintf(const char* fmt, ...) {
  static char strbuf[32] = {0};
  va_list args;
  va_start(args, fmt);
  vsnprintf(strbuf, sizeof(strbuf), fmt, args);
  va_end(args);
  return strbuf;
}

void doReceiveLoop() {
      // try to parse packet
  static uint8_t packet[256];
  uint8_t length = 0;
  // save room in recv() for '\0'
  if ((length = lora->recv(packet, sizeof(packet) - 1)) > 0) {
        // received a packet
        packet[length] = '\0';
        if (u8g2) {
            u8g2->clearBuffer();
            u8g2->drawStr(0, 12, "Received OK!");
            u8g2->drawStr(0, 26, (char*)packet);
            u8g2->drawStr(0, 40, Sprintf("RSSI:%i", lora->lastRssi()));
            u8g2->drawStr(0, 56, Sprintf("SNR:%.1f", lora->lastSnr()));
            u8g2->sendBuffer();
        }
    }
}

void doSendLoop() {
  static int counter = 0;
  Serial.print("Sending packet: ");
  Serial.println(counter);

  char* msg = Sprintf("hello %d", counter);
  lora->send((uint8_t*)msg, strlen(msg));

  if (u8g2) {
      char buf[256];
      u8g2->clearBuffer();
      u8g2->drawStr(0, 12, "Transmitting: OK!");
      snprintf(buf, sizeof(buf), "Sending: %d", counter);
      u8g2->drawStr(0, 30, buf);
      u8g2->sendBuffer();
  }
  counter++;
}