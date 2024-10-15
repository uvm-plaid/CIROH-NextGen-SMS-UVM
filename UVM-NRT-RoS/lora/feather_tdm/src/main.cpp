#include "loraDevice.h"
#include "tdm.h"
#include <Arduino.h>

#if defined(USING_SX1276) || defined(USING_SX1278)
#include <U8g2lib.h>
#include "platform/lilygo/LoRaBoards.h"
#else
#include "platform/feather/rf95.h"
#endif

const double FREQ = 915E6;

/*
// Change this const to change board deploy to Tx or Rx.
const vtable_t tab = tx_vtable;

int counter = 0;

// All this stuff is for the Arduino MKLAN 1030 devices.
// Not sure where to put it yet.
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

const bool isReceiving = false;
#ifndef USING_SX1276
vtable_t tab = isReceiving ? rx_vtable : tx_vtable;
#endif

LoraDevice* lora = nullptr;

void setup() {
#ifdef USING_SX1276
  lora = LoraDevice::getInstance();
  lora->setup();

  if (u8g2) {
      u8g2->clearBuffer();
      u8g2->drawStr(0, 12, "Awaiting first msg");
      u8g2->sendBuffer();
  }
#else
  tab.setup(rf95);
#endif
}

void doReceiveLoop();
void doSendLoop();

void loop() {
#ifdef USING_SX1276
  if (isReceiving) {
    doReceiveLoop();
  } else {
    doSendLoop();
  }
#else
  tab.loop(rf95);
#endif
}

void doReceiveLoop() {
  static uint8_t packet[256];
  int32_t length = 0;
  // save room in recv() for '\0'
  if ((length = lora->recv(packet, sizeof(packet) - 1)) > 0) {
        // received a packet
        packet[length] = '\0';
        //Serial.print("Recv() returned: ");
        //Serial.println(length);
        //Serial.println((char*)packet);
        #if defined(USING_SX1276) || defined(USING_SX1278)
        if (u8g2) {
            u8g2->clearBuffer();
            u8g2->drawStr(0, 12, "Received OK!");
            u8g2->drawStr(0, 26, (char*)packet);
            u8g2->drawStr(0, 40, Sprintf("RSSI:%i", lora->lastRssi()));
            u8g2->drawStr(0, 56, Sprintf("SNR:%.1f", lora->lastSnr()));
            u8g2->sendBuffer();
        }
        #endif
    } else if (length < 0) {
#if defined(USING_SX1276) || defined(USING_SX1278)
      const char* errorMsg = "";
      if (length == -1) {
        errorMsg = "Recv() failed";
      } else if (length == -2) {
        errorMsg = "Device Unav.";
      } else {
        errorMsg = "Unknown";
      }
      if (u8g2) {
          u8g2->clearBuffer();
          u8g2->drawStr(0, 12, "Receive FAIL!");
          u8g2->drawStr(0, 26, errorMsg);
          u8g2->sendBuffer();
      }
#endif
      //Serial.print("Recv() returned: ");
      //Serial.println(length);
    }
}

void doSendLoop() {
  static int counter = 0;

  char* msg = Sprintf("hello %d", counter);
  int32_t bytesSent = 0;
  bytesSent = lora->send((uint8_t*)msg, strlen(msg));
  if (bytesSent > 0) {
    Serial.print("Message successfully sent: ");
    Serial.println(counter);
  } else if (bytesSent == -1) {
    Serial.println("Message failed to send.");
  } else if (bytesSent == -1) {
    Serial.println("Device unavailable.");
  }

#if defined(USING_SX1276) || defined(USING_SX1278)
  if (u8g2) {
      static char buf[256];
      u8g2->clearBuffer();
      const char* errorMsg = 0;
      if (bytesSent > 0) {
        errorMsg = "OK!";
      } else if (bytesSent == -1) {
        errorMsg = "FTS"; // failed to send
      } else if (bytesSent == -2) {
        errorMsg = "UNA"; // Device unavailable
      } else {
        errorMsg = "UNK"; // Unknowon error occurred
      }
      u8g2->drawStr(0, 12, Sprintf("Transmitting: %s", errorMsg));
      snprintf(buf, sizeof(buf), "Sending: %d", counter);
      u8g2->drawStr(0, 30, buf);
      u8g2->sendBuffer();
  }
#endif
  counter++;
}