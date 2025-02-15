#include "LoraDevice.h"
#include "tdm.h"
#include <Arduino.h>

#if defined(USING_SX1276) || defined(USING_SX1278)
#include <U8g2lib.h>
#include "LoRaBoards.h"
#else
#include "rf95.h"
#endif

void beginDisplayStr();
void displayStr(const char* line);
void displayf(const char* fmt, ...);
void endDisplayStr();
void displayLines(const char* lines[], int numLines);
void displayLines(const char* line1="", const char* line2="", const char* line3="", const char* line4="");

/*
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

constexpr uint32_t DeviceID = 0;
constexpr bool isGateway = (DeviceID == 0);

static LoraDevice* lora = nullptr;

void setup() {
  lora = LoraDevice::getInstance();
  lora->setup();

  displayLines("Awaiting first msg");
}

void doGatewayLoop();
void doSendLoop();

void windowDisplay() {
  static int lastNode = 0;
  static int lastNodeStart = 0;
  static int lastNodeDuration = 0;
  if (currentNode() != lastNode) {
    lastNodeDuration = nowMs() - lastNodeStart;
    lastNodeStart = nowMs();
    lastNode = currentNode();
  }
  beginDisplayStr();
  displayf("Current Node: %d", currentNode());
  displayf("t=%d", nowMs());
  displayf("d_last=%d", lastNodeDuration);
  endDisplayStr();
}

void loop() {
  if (isGateway) {
    doGatewayLoop();
    windowDisplay();
  } else {
    doSendLoop();
  }
}


void doGatewayLoop() {
  static uint8_t packet[256];
  static char bytesBuf[16];
  int32_t length = 0;
  const int timeNode = 0;
  if (isCurrentNode(timeNode)) {
    uint8_t* buffer = nullptr;
    if (!sendTime(&buffer)) {
      displayLines("Failed to send time!");
    } else {
      snprintf(bytesBuf, sizeof(bytesBuf), "%x %x %x %x %x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
      displayLines("Sent time at", Sprintf("%dms", millis()), bytesBuf);
    }
  }
  else if ((length = lora->recv(packet, sizeof(packet) - 1)) > 0) {
      // received a packet
      packet[length] = '\0';
      //Serial.print("Recv() returned: ");
      //Serial.println(length);
      //Serial.println((char*)packet);
      beginDisplayStr();
      displayStr("Received OK!");
      displayStr((char*)packet);
      displayf("RSSI:%i", lora->lastRssi());
      displayf("SNR:%.1f", lora->lastSnr());
      endDisplayStr();
  }

  if (length < 0) {
      const char* errorMsg = "";
      if (length == -1) {
        errorMsg = "Recv() failed";
      } else if (length == -2) {
        errorMsg = "Device Unav.";
      } else {
        errorMsg = "Unknown";
      }
      displayLines("Receive FAIL!", errorMsg);
  }
}

void doSendLoop() {
  static int counter = 0;
  static bool hasConnected = false;

  if (!isConnected()) {
    uint8_t* buffer;
    int error = recvTime(&buffer);
    switch (error) {
      case -1:
        displayLines("Received non-time", "packet.", Sprintf("%d", buffer[0]));
        break;
      case -2:
        displayLines("Received invalid", "time packet.", Sprintf("%x", *(uint32_t*)&buffer[1]));
        break;
    }
    if (error == 1) {
      displayLines("Received time", "packet!");
    } else {
      return;
    }
  } else if (!hasConnected) {
    hasConnected = true;
    displayLines("Connected.");
  }

  if (isCurrentNode(DeviceID)) {
    // My turn to talk!
    const char* msg = Sprintf("hello %d", counter);
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
    beginDisplayStr();
    displayf("Transmitting: %s", errorMsg);
    displayf("Sending: %d", counter);
    displayf("t=%d", nowMs());
    endDisplayStr();
    Serial.print("t=");
    Serial.println(nowMs());
    counter++;
  } else if (isCurrentNode(0)) {
    recvTime();
  }
}


/// Functions only for devices with displays below.

#if defined(USING_SX1276) || defined(USING_SX1278)
static int lineY = 12;
#endif

void beginDisplayStr()
{
#if defined(USING_SX1276) || defined(USING_SX1278)
  if (!u8g2) return;
  u8g2->clearBuffer();
  lineY = 12;
#endif
}

void displayStr(const char* line)
{
#if defined(USING_SX1276) || defined(USING_SX1278)
  if (!u8g2) return;
  if (lineY > 56) return;
  u8g2->drawStr(0, lineY, line);
  lineY += 14;
#endif
}

void displayf(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  const char* str = Vsprintf(fmt, args);
  va_end(args);
  displayStr(str);
}

void endDisplayStr()
{
#if defined(USING_SX1276) || defined(USING_SX1278)
  if (!u8g2) return;
  u8g2->sendBuffer();
#endif
}

void displayLines(const char* lines[], int numLines)
{
#if defined(USING_SX1276) || defined(USING_SX1278)
  if (numLines > 4) {
    // Can't fit more than 4 lines on the display
    numLines = 4;
  }
  if (!u8g2) return;

  u8g2->clearBuffer();
  int lineY = 12;
  for (int i = 0; i < numLines; i++) {
    u8g2->drawStr(0, lineY, lines[i]);
    lineY += 14;
  }
  u8g2->sendBuffer();

#endif
}

void displayLines(const char* line1, const char* line2, const char* line3, const char* line4) {
#if defined(USING_SX1276) || defined(USING_SX1278)
  if (!u8g2) return;

  u8g2->clearBuffer();
  int lineY = 12;

  u8g2->drawStr(0, lineY, line1);
  lineY += 14;

  u8g2->drawStr(0, lineY, line2);
  lineY += 14;

  u8g2->drawStr(0, lineY, line3);
  lineY += 14;

  u8g2->drawStr(0, lineY, line4);
  lineY += 14;
  
  u8g2->sendBuffer();

#endif

}
