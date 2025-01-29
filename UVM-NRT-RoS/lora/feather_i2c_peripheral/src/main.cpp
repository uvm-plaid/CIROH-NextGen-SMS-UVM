#include "loraDevice.h"
#include "tdm.h"
#include <Arduino.h>
#include <Wire.h>

#if defined(USING_SX1276) || defined(USING_SX1278)
#include <U8g2lib.h>
#include "platform/lilygo/LoRaBoards.h"
#else
#include "platform/feather/rf95.h"
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
constexpr bool isPeripheral = isGateway;
constexpr uint8_t peripheral_address = 0x08;  // Hardcoded for now

static LoraDevice* lora = nullptr;
LoraPeripheral* LoraPeripheral::instance = nullptr;
static LoraPeripheral* peripheral = nullptr;

/**
 * This function is called whenever a device running the I2C controller
 * requests from the feather.
 * 1) Requests a packet from the packet buffer (early return if empty).
 * 2) Serializes packet into buffer (eliminate any padding from compiler).
 * 3) Write buffer to receiver.
 */
void i2c_request_handler() {
    static uint8_t buf[sizeof(LoraPacket) + 1];
    LoraPacket packet;
    LoraPeripheralStatus status = peripheral->request(packet);
    if (status == LoraPeripheralStatus::BUFFER_EMPTY) {
        packet.flags = LoraPacketFlags::NO_MORE_PACKETS;
        return;
    }
    uint32_t nbytes = 0;
    LoraPacket::SerDeStatus ret = packet.serialize(buf, sizeof(buf), nbytes);
    Serial.println(static_cast<uint8_t>(status));
    if (ret == LoraPacket::SerdeStatus::Valid) {
        Wire.write(buf, nbytes);
    }
}

void i2c_initiate() {
    peripheral = LoraPeripheral::get_instance();
    if (isPeripheral) {
        Wire.begin(peripheral_address);
        Wire.onRequest(i2c_request_handler);
        Serial.println("Setup I2C server.");
    } else {
        // This code should only be running on a server so loop
        while (true);
    }
}


void setup() {
    i2c_initiate();
}

void loop() {
    LoraPacket packet;
    for (int i = 0; i <= 10; ++i) {
        packet.source_id = i; 
        LoraPeripheralStatus status = peripheral->receive(packet);
        if (status == LoraPeripheralStatus::RECEIVE_SUCCESSFUL) {
            Serial.println("Successfully received packet");
        }
        delay(i * 50);
    }
}

